#include <TSystem>

class StMaker;
class StChain;
class StPicoDstMaker;

StChain *chain;
void readPicoDst(Int_t nEvents=100, const Char_t *inputFile="test.list", const TString outputFile="test/test.root", const Bool_t debug = kFALSE)
{
        //Load all the System libraries
        gROOT->LoadMacro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
        loadSharedLibraries();

        gSystem->Load("StPicoDstMaker");
        gSystem->Load("StMiniTreeMaker");

        chain = new StChain();

        StPicoDstMaker *picoMaker = new StPicoDstMaker(0,inputFile,"picoDst");

        StPicoMtdCalibMaker *calibMaker = new StPicoMtdCalibMaker();
        calibMaker->setInitFromFile(kTRUE);
        calibMaker->setApplyT0(kTRUE);
        calibMaker->setCalibFileT0("StRoot/StPicoDstMaker/Run14_AuAu200_CalibDtof.offline.dat");

        StMiniTreeMaker *miniTreeMaker = new StMiniTreeMaker();
        miniTreeMaker->setOutFileName(outputFile);
        miniTreeMaker->setPrintCpu(0);
        if(debug)
                miniTreeMaker->SetDebug(1);


        if(chain->Init()==kStERR) return;
        cout<<"chain->Init();"<<endl;

        if(nEvents<0) nEvents = picoMaker->chain()->GetEntries();

        cout << "****************************************** " << endl;
        cout << "total number of events  " << nEvents << endl;
        cout << "****************************************** " << endl;

        for(Int_t i=0; i<nEvents; i++) {
                if(debug) {
                        cout<<"Working on eventNumber "<< i <<endl;
                } else {
                        if(i%1000==0)
                                cout << "Working on eventNumber " << i << endl;
                }

                chain->Clear();
                int iret = chain->Make(i);

                if(iret) { cout << "Bad return code!" << iret << endl; break;}
        }

        cout << "****************************************** " << endl;
        cout << "Work done... now its time to close up shop!"<< endl;
        cout << "****************************************** " << endl;

        chain->Finish();
        delete chain;
}

