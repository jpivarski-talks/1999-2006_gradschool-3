// ******************************************************** 
// stand-alone testing program for the DataDelivery system
// without Suez!
// ******************************************************** 

#include <iostream.h>
#include <stdlib.h>  // required for 'exit'
#include <String.h>

#include "ToolBox/Include/functions.h" // for report

// user include files
#include "SyncFrame.h"
#include "Switchboard/Include/Frame.h"
#include "Switchboard/Include/EventRecord.h"
#include "Switchboard/Include/DataElement.h"
#include "Switchboard/Include/DataTable.h"

#include <vector>

#include "FrameDeliverer.h"
#include "KarpDataBinder.h"



//------ anal1 -----
//
void anal1( Frame& )
{
}


//------ anal2 -----
//
void anal2( Frame& )
{
}





//------ anal3 -----
//
void anal3( Frame& iFrame )
{
   const EventRecord& event = iFrame.event();

   if( event.contains( EventElements::kPionFit ) ){
      const DataElement& pionFit = event.dataElement( EventElements::kPionFit);

      DataTable pionFitTable( pionFit.size() );
      if( pionFit.fillTable( pionFitTable) ){
	 cout <<"Number of Pions:"<<pionFitTable.height()<<endl; 
      } else {
	 //table didn't fill
	 cout << "PionTable not filled"<<endl;
      }
   } else {
      //Event record doesn't have pion fit
      cout << "Event record doesn't have pion fit"<<endl;
   }

}

//------ anal4 -----
//
void anal4( Frame& )
{
}


//------ anal5 -----
//
void anal5( Frame& )
{
//   clearSpeciesTable() ;
}


// -----------------------------------------------------------------
// main routine

int main(int argc, char *argv[]){

   FrameDeliverer FD;
      
   //Bind the source to the stream
   AsciiBinder
   KarpDataBinder karpDataBinder = KarpDataBinder( hostname, port,
						   schema, dbname,
						   numComponents, 
						   Stream::kBeginRun |
						   Stream::kEvent |
						   Stream::kEndRun,
						   true );

   //Open the source requested
   FrameDeliverer::AddDataSourceError iError;
      
   if( ! FD.addDataSource( karpDataBinder, iError) ){
      switch(iError){
	 case FrameDeliverer::kInvalidStopsForSource:
	 cout << "Source can't goto those stops"<<endl;
	 break;
	 case FrameDeliverer::kSourceDidntLikeInfo:
	 cout << "Couldn't open data source "<<endl;
	 break;
	 case FrameDeliverer::kConflictingStopResponsibility:
	 cout << "Another source already goes to that stop"<<endl;
	 break;
      }
      exit(1);
   }
      
      
   cout <<"Opened data stream"<<endl;
   anal1() ;
      
   FD.setActiveStreams( Stream::kBeginRun |
      Stream::kEvent |
      Stream::kEndRun 
      );
      
   //Before running, check to see if we have sources for all the stops we
   //  requested.
   if(FD.needMoreSources()){
      cout<<"Need more sources"<<endl;
      exit(0);
   }
      
   // request a certain SynchronizationValue (run, event here)
   unsigned long run, evnt;
   cout << "Input first run, evnt" << endl;
   cin >> run >> evnt;
   if( ! FD.synchronizeSources( SyncValue( run, evnt ) ) ) {
      report(EMERGENCY) << "---couldn't synchronize Sources---" << endl;
      exit(1);
   }


   cout <<"Starting loop"<<endl;
      
   int isDone = 0;
   while(! isDone ){
      Stream::Type currentRecord = FD.nextStop( ) ;
      switch( currentRecord){
	 case Stream::kNone:
	    report( EMERGENCY ) <<"---Done---"<<endl;
	    isDone = 1;
	    break;
	 case Stream::kBeginRun:
	    cout <<"---BeginRun---"<<endl;
            // anal2(FD.currentFrame());
	    FD.currentFrame();
	    break;
	 case Stream::kEvent:
	    cout <<"---Event---"<<endl;
	    // anal3(FD.currentFrame()) ;
	    FD.currentFrame();
	    break;
	 case Stream::kEndRun:
	    cout <<"---EndRun---"<<endl;
	    break;
	 default:
	    report( EMERGENCY )<<"Unknown record type:"<<currentRecord<<endl;
	    isDone = 1;
	    break;
      }
      if(!isDone){
	 char wait;
	 cout<<"Next Record?";
	 cin >>wait;
	 if('n'== wait){
	    isDone = 1;
	 }
      }
	 
   }
   anal5() ;
      
   FrameDeliverer::RemoveDataSourcesError iCloseError;
   if( ! FD.removeDataSources(Stream::kEvent,
			      iCloseError) ){
      switch(iCloseError){
	 case FrameDeliverer::kCloseStopsNotValid:
	 cout << "CloseDataSource: Stop to be deactivated was not active" <<endl;
	 break;
      }
      exit(1);
   }
      
}
  

#if 0
//////////////////////////////////////////////////////////////////////


DataSourceManager *dataSourceManager = 
(*dataBinder).createDataSourceManager();

if( !(*dataSourceManager).setRecordsToRetrieve( Stream::kBeginRun |
						Stream::kEvent |
						Stream::kEndRun ) ) {
   cout << "couldn\'t set setRecordsToRetrieve" << endl;
}

// try out synchronization
cout << "trying out Synchronization feature" << endl;
cout << "input run/event number (have to be the same for now)" << endl;
int runn, eventn;
cin >> runn >> eventn;

if( !(*dataSourceManager).synchronizeSource( SyncValue( runn, eventn ) ) ) {
   cerr << "couldn't synchronize" << endl;
   exit(-1);
}

int isDone = 0;
while(! isDone ){
   Stream::Type typeOfNextRecord = 
      (*dataSourceManager).gotoNextRecord( Stream::kEvent |
					   Stream::kEndRun );
   switch( typeOfNextRecord ) {
      case Stream::kNone:
      cout <<"---Done---"<<endl;
      isDone = 1;
      break;
      case Stream::kBeginRun:
      cout <<"---BeginRun---"<<endl;
      break;
      case Stream::kEvent:
      cout <<"---Event---"<<endl;
      break;
      case Stream::kEndRun:
      cout <<"---EndRun---"<<endl;
      break;
      default:
      cout << "Unknown record type:" << typeOfNextRecord << endl;
      isDone = 1;
      break;
   }

   if(!isDone){

      // now look at the records for data
      Record *record=(*dataSourceManager).createRecord( typeOfNextRecord );
      cout << (*record).identifier() << ": [";
      
      // first component is Stream::Type; skip!
      for(int i=1; i<iNumComponents; ++i) {
	 int* comp = (int*)(*record).component(i);
	 if(comp == 0) continue;
	 for(int j=0; j<3; ++j) {
	    cout << " ";
	    cout << comp[j];
	 }
      }
      cout << " ]\n" << flush;
      //delete record; // eventually have to do something smarter here


      // read more records?
      char wait;
      cout<<"Next Record?";
      cin >>wait;
      if('n'== wait){
	 isDone = 1;
      }
   }
      

}

delete dataSourceManager;
delete dataBinder;
      
}
#endif

