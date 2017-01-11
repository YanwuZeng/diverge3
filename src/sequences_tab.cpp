#include <string>
#include <vector>

#ifndef WIN32
#include <values.h>
#endif

#include <assert.h>

#include <qfiledialog.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

#include "sequences_tab.h"
#include "sequence.h"
#include "common.h"
#include "alignment_view.h"
#include "align.h"
#include "pdb.h"
#include "pdb_viewer.h"
#include "progress.h"

//----------------------------------------------------------------------

//these parameters have already been defined in Main.cpp:compilar->link->execute
using namespace std;

extern vector<sequence_t> orig_sequences;
extern vector<sequence_t> sequences;
extern vector<int> removed;
extern PDB pdb;
extern char pdb_chain_ID;
extern vector<string> pdb_map;

//----------------------------------------------------------------------

//SequencesTab inherits QWidget, and use constructor of QWidget first.
SequencesTab::SequencesTab(QWidget *parent, const char *name) 
: QWidget(parent, name)
{
	//Use Qt Verical Box Layout
	QVBoxLayout *top_layout = new QVBoxLayout(this);
	top_layout->setMargin(10);//Sets the width of the outside border of the layout. 
	
	//Use Qt Horizontal Box Layout named seq_box:
	QHBox *seq_box = new QHBox(this);
	seq_box->setSpacing(10);//Sets the spacing between widgets inside the layout
	//add PushButton"Load Alignment" and Widget using the QHBox format:
	QPushButton *pb1 = new QPushButton("Load &Alignment", seq_box);
	pb1->setFixedWidth(pb1->sizeHint().width());
	seq_entry_ = new AlignmentView(seq_box);
	top_layout->addWidget(seq_box);
	
	//add signal to PushButton
	connect(pb1, SIGNAL(clicked()), SLOT(load_alignment()));
	
	top_layout->addSpacing(10);//Adds a non-stretchable space with size size to the end of this box layout. QBoxLayout provides default margin and spacing. This function adds additional space.
	
	//Use Qt Horizontal Box Layout named pdb_box:
	QHBox *pdb_box = new QHBox(this);
	pdb_box->setSpacing(10);
	//add PushButton"Load Structure" and Widget using the QHBox format:
	QPushButton *pb2 = new QPushButton("Load &Structure", pdb_box);
	pb2->setFixedWidth(pb1->sizeHint().width());
	pdb_alignment_ = new AlignmentView(pdb_box);
	pdb_alignment_->setFixedHeight(150);
	
	pdb_box->setFixedHeight(pdb_box->minimumSizeHint().height());
	
	top_layout->addWidget(pdb_box);
	//add signal to PushButton
	connect(pb2, SIGNAL(clicked()), SLOT(load_pdb()));
	
	top_layout->activate();
}

//----------------------------------------------------------------------

//deconstuctor
SequencesTab::~SequencesTab() {}

//----------------------------------------------------------------------

//when load_alignment, it will popup a window asking you to choose a file,
//if a false value, it will return Null. Otherwise, return a file name.
void
SequencesTab::load_alignment() {
	QString s(QFileDialog::getOpenFileName(QString::null, "Alignment Files (*.aln;*.fasta);;Clustal Alignment (*.aln);;Fasta (*.fasta);;All Files (*)", NULL, NULL, "Open alignment file"));
	if(s.isNull()) return;
	
	load_alignment((const char*)s);
}

//----------------------------------------------------------------------
//load_alignment() is a method which will open the file (filename),
// and store the alignment info using internal data variables.
bool
SequencesTab::load_alignment(string filename) {
	vector<sequence_t> tmp_seqs;
	
	//Call the load_sequences() method of class sequence to 
	//load the sequences into temp_seqs
	//if failed, print out the popup error messages.
	if(!load_sequences(filename.c_str(), tmp_seqs)) {
		QMessageBox::critical(NULL, appname, QString("Unable to load ") +
			QString(filename.c_str()));
		return false;
	}
	
	sequences = orig_sequences = tmp_seqs;
	
	//call the clean_gaps() of class sequence (sequence.cpp) to 
	//clean the gaps of original sequences. The new sequence will
	//be stored in variable (removed).
	clean_gaps(sequences, removed);
	
	//set the orig_sequences to be used in the alignment_view
	seq_entry_->set_alignment(orig_sequences);
	
	//update the PDB map between protein structure info. and 
	//gene alignment data.
	updatePDBMap();
	
	//emit the signal to inform other window tabs to update
	//their internal information.
	emit(alignmentChanged());
	
	return true;
}

//----------------------------------------------------------------------

class AlignmentResult {
public:
	AlignmentResult() : score(INT_MIN) {}
	AlignmentResult(const AlignmentResult &ar) : score(ar.score), seq(ar.seq),
		chain(ar.chain), as1(ar.as1),
		as2(ar.as2) {}
	AlignmentResult & operator = (const AlignmentResult &ar) {
		score = ar.score;
		seq   = ar.seq;
		chain = ar.chain;
		as1   = ar.as1;
		as2   = ar.as2;
		return *this;
	}
	
	int score;
	vector<sequence_t>::const_iterator seq;
	vector<Chain>::const_iterator chain;
	string as1;
	string as2;
};

void
SequencesTab::updatePDBMap() {
	if(pdb.chains.empty()) {
		return;
	}
	
	pdb_map.clear();
	if(sequences.empty()) return;
	
	int max_score = INT_MIN;
	QStringList max_chain_IDs;
	vector<AlignmentResult> alignments;
	
	
	Progress::setup(0, pdb.chains.size() * sequences.size(), "Aligning...");
	Progress::show();
	
	int n = 0;
	vector<Chain>::const_iterator i;
	for(i = pdb.chains.begin(); i != pdb.chains.end(); i++) {
		string pdb_seq = i->sequence();
		if(pdb_seq.empty()) continue;
		vector<sequence_t>::const_iterator j;
		for(j = orig_sequences.begin(); j != orig_sequences.end(); j++, n++) {
			Progress::increment();
			if(Progress::wasCancelled()) {
				return;
			}
			
			string as1, as2;
			int score = align(j->sequence, pdb_seq, as1, as2);
			if(score > max_score) {
				max_score = score;
				max_chain_IDs.clear();
				alignments.clear();
			}
			if(score == max_score /*&& !max_chain_IDs.contains(QString(QChar(i->name)))*/) {
				max_chain_IDs.append(QString(QChar(i->name)) + QString(" -  ") + QString(j->label.c_str()));
				
				AlignmentResult ar;
				ar.score = score;
				ar.seq = j;
				ar.chain = i;	
				ar.as1 = as1;
				ar.as2 = as2;
				alignments.push_back(ar);
			}
		}
	}
	
	Progress::hide();
	
	assert(!max_chain_IDs.isEmpty());
	
	int ind = 0;
	if(max_chain_IDs.count() > 1) {
		bool ok;
		QString str = QInputDialog::getItem(appname, "Select Optimal Chain", max_chain_IDs, 0, false, &ok);
		if(!ok) return;
		ind = max_chain_IDs.findIndex(str);
		assert(ind != -1);
	}
	
	const AlignmentResult &ar = alignments[ind];
	if(ar.chain->sequence().empty()) return;
	
	vector<string> pdb_res_nums = ar.chain->res_nums();
	
	int len = ar.seq->sequence.size();
	int len2 = ar.as1.size();
	int len3 = pdb_res_nums.size();
	pdb_map.resize(len);
	for(int j=0, k=0, l=0; j<len; j++) {
		if(ar.seq->sequence[j] == '-') continue;
		while(k < len2 && ar.as1[k] != ar.seq->sequence[j]) {
			if(ar.as2[k] != '-') l++;
			k++;
		}
		if(k < len2) {
			if(ar.as2[k] != '-') {
				if(l < len3) {
					pdb_map[j] = pdb_res_nums[l];
				}
				l++;
			}
		}
		k++;
	}
	
	pdb_chain_ID = ar.chain->name;
	
	{
		vector<sequence_t> seqs;
		
		sequence_t seq;
		
		seq.label = ar.seq->label;
		seq.sequence = ar.as1;
		seqs.push_back(seq);
		
		seq.label = pdb.name;
		if(pdb_chain_ID != ' ') {
			seq.label += '_';
			seq.label += pdb_chain_ID;
		}
		seq.sequence = ar.as2;
		seqs.push_back(seq);
		
		pdb_alignment_->set_alignment(seqs);
	}
	
	pdb_load(pdb);
}

//----------------------------------------------------------------------

//when load_pdb, it will popup a window asking you to choose a file,
//if a false value, it will return Null. Otherwise, return a file name.
void
SequencesTab::load_pdb() {
	QString s(QFileDialog::getOpenFileName(QString::null, "PDB (*.pdb *.ent);;All Files (*)", NULL));
	if(s.isNull()) return;
	//if load a new correct pdb file, update PDBMap, otherwise show warning mesg "Failed to load PDB file!" 
	pdb.clear();
	if(pdb.load((const char *)s)) {
		updatePDBMap();
	} else {
		QMessageBox::warning(NULL, appname, "Failed to load PDB file!");
	} 
}

//----------------------------------------------------------------------
