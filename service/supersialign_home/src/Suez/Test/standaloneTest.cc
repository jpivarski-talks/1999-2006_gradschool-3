#include "Experiment/Experiment.h"

#include <iostream.h>
#include <map>
#include <vector>
#include <string>

// basic datadelivery 
#include "DataHandler/Stream.h"
#include "DataDelivery/FrameDeliverer.h"
#include "DataHandler/StreamSet.h"

// frame access
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"

// cleo objects
#include "CleoDB/DBEventHeader.h"
#include "CleoDB/DBPhotonFit.h"
#include "CompositeDelivery/COPionFit.h"

// binders for different file types
#include "AsciiDelivery/AsciiSourceBinder.h"
#include "DriverDelivery/DriverSourceBinder.h"

#include <map>
#include <vector>

// ---------- event processing routine -------------
void eventProcess( const Frame& iFrame)
{
   FAItem< DBEventHeader > eventHeader;
   extract( iFrame.record( Stream::kEvent ), eventHeader);
   int run = (*eventHeader).run();
   int event = (*eventHeader).number();

   if( eventHeader.valid() )
   {
      cout <<"Run/Event:"
	   <<(*eventHeader).run()
	   <<"/"
	   <<(*eventHeader).number()
	   <<endl;
   } 
   else 
   {
      cout <<"Invalid Event Header"<<endl;
   }


   FATable< COPionFit > pionFits ;
   extract( iFrame.record( Stream::kEvent ), pionFits ) ;
   if( pionFits.valid() )
   {
      cout <<"  Number of tracks:"<< pionFits.size() <<endl;
   }
   
   return;
}

// ------------------------- Main Program ---------------------------
int main( void )
{
   //Set up the message logging system
   //LogStream logger ( INFO ,
   //                   cout ) ;
   //MessageLog::Tie( "." , logger ) ;


   Stream::Set readStreams;
   readStreams.insert( Stream::kEvent);

   FrameDeliverer frameDeliverer;

   FrameDeliverer::AddSourceStatus iAddError;
   FrameDeliverer::AddActiveSourceStatus iActiveError;

   char fileName[256];

   Stream::Set readRunEvent;
   readRunEvent.insert( Stream::kBeginRun);
   readRunEvent.insert( Stream::kEvent );
   cout <<"ASCII Source File Name:";
   cin >> fileName;

   AsciiSourceBinder asciiBinder( fileName,
				  readRunEvent);

   // declare binder for reading
   if( FrameDeliverer::kAddSourceNoError !=
       (iAddError = frameDeliverer.addSource( asciiBinder))){
      cout <<"Add Data Source error:"<< iAddError <<endl;
      exit(1);
   }

   // declare binder for active streams
   if( FrameDeliverer::kAddActiveSourceNoError != 
       (iActiveError = frameDeliverer.addActiveSource( asciiBinder))){
      cout <<"Add Active Stream error:"<< iActiveError <<endl;
      exit(1);
   }


   DABoolean isDone(false);
   FrameDeliverer::NextStopStatus nextStopStatus;

   int nEvents(0);
   while( !isDone )
   {
      nextStopStatus = frameDeliverer.gotoNextStop();
      Stream::Type currentStop = frameDeliverer.currentStop();
      
      cout << "------------------------" << endl;
      cout << "NextStopStatus:" << nextStopStatus << endl;

      if( Stream::kNone == currentStop )
      {
	 isDone = true;
      } 
      else if ( Stream::kBeginRun == currentStop )
      {
	 cout <<"Begin Run"<<endl;
      } 
      else if ( Stream::kEvent == currentStop )
      {
	 ++nEvents;
	 cout <<"Event"<<endl;
	 eventProcess( frameDeliverer.currentFrame() );
      } 
      else if ( Stream::kEndRun == currentStop )
      {
	cout <<"End Run"<<endl;
      } 
      else 
      {
	 cout <<"Unknown Record:"<<currentStop<<endl;
	 isDone=true;
      }
   }
      
   cout << "------------------------" << endl;
   cout << "Next Stop Status ( End of Active Source="
	<< FrameDeliverer::kNextStopReachedEndOfAnActiveSource
	<< "):"
	<<nextStopStatus
	<<endl;
      
   cout << nEvents<<endl;
   return 0;
}


