#include "multiplayerSync.h"
#include "shark.h"
#include "boat.h" //already included in GameController i think but works as a remainder
#include "test7.h"
//#include "uart.h"

#include "smart_vector.h"
#include "stdlib.h"

//easier to destroy only one shark->fixed received data size
//typedef unsigned int destroyedSharkIndex;
//defineSmartVector(destroyedSharkIndex);

#include "Utilities.h" //debug

/** @defgroup multiplayerSync multiplayerSync
 * @{*/

/**@note:PROTOCOL
 * sync_seed-> [start sign (not really needed but avoids creating an other func)]  [8 seed] x6 (bytes reversed->msbyte is on last char) [end]
 * ingame-> [start sign] [4 boat x,4 boat y] [8 boat x] [8 boat y] [7 shark index, 1 gameover] [end]
 */

char MULTIPLAYER_USER_TYPE = 0;///<indicates if the user is the host or the client. default is None
const unsigned short HOST_TIMEOUT = 30;///<if host waits for an answer this number of thirds, should switch to "send mode"
unsigned short timeout_counter = 0;///<counts thirds until message is received
char sync_state = 0; ///<if it is waiting or sending a message

unsigned char received_info[30] ;
unsigned int numberOfBytesReceived=0;

const char OK = 0; // did not include =P #include <minix/drivers.h>

//destroyedSharkIndex_SV* destroyedSharks;
GameController** game;///<pointer to game instance

#if(1) //region:"basic" funcs

char multiplayer_getUserType()
{return MULTIPLAYER_USER_TYPE;}

void multiplayer_defineUserType(unsigned char user_type, GameController** game_p)
{
	game=game_p;
	MULTIPLAYER_USER_TYPE = user_type;
	//one user starts sending and the other receiving
	sync_state= MULTIPLAYER_USER_TYPE==USER_TYPE_HOST ?
			MULTIPLAYER_SYNC_RECEIVE:MULTIPLAYER_SYNC_SEND;
	uart_startMode();
}

char multiplayer_receive()
{
	/* forces the host to go into send mode if it waits to much time without
	 * receiving anything. Avoids getting both users stuck waiting for each other
	 * */
	char answer=0;
	if ( (answer=uart_receive(received_info,&numberOfBytesReceived) )!=OK)
	{
		//LOG_SDEC("uart_notOk. Ret:",answer);
		if (MULTIPLAYER_USER_TYPE==USER_TYPE_HOST){
			timeout_counter++;
			//if (timeout_counter>=HOST_TIMEOUT) sync_state=MULTIPLAYER_SYNC_SEND;
		}
		//avoid storing end signs when not receiving anything
		if (answer==2)
		{
			DEBUG_MSG("multiplayer_receive no start");
			multiplayer_reset_receivedInfo();
		}
	}
	//else process_received_inGameData();//==OK (received all info)

	DEBUG_MSG("multiplayer_receive e");
	return answer;
}

void multiplayer_reset_receivedInfo()
{
	numberOfBytesReceived=0;
	unsigned int i=0;
	for (;i<30;i++) received_info[i] = ' ' ;
}

#endif //region:basicfunc


#if(1) //region:seed sync funcs

char getSeed(unsigned int *seed)
{
	timeout_counter=0;
	unsigned int received_seed=0;

	//convert received string to seed
	//using 8 bits "packs"
	//note received[0] should be start symbol
	DEBUG_MSG("getSeed 0");
	unsigned int i=2;
	for(;i>0;i--)
	{
		received_seed+=( ( (unsigned int) received_info[i] ) & 0x00FF) << ((i-1)*8) ;//8=number of bits in a pack
	}

	*seed=received_seed;


	DEBUG_MSG("getSeed 1");
	LOG_UDEC("receiveSeed seed:", (*seed) );

	multiplayer_reset_receivedInfo();

	DEBUG_MSG("getSeed 2");
	LOG_SDEC("ended getSeed ok:",0);

	return 0;
}

char sendSeed(unsigned int *seed)
{
	LOG_UDEC("sendSeed seed:", (*seed) );

	unsigned char* seedStr; ALLOC(unsigned char,2,seedStr);

	//build string to send
	unsigned int i=2;
	for(;i>0;i--)
	{
		seedStr[i-1]= ( (*seed) >> ((i-1)*8)  ) & 0x00FF;
		DEBUG_UDEC(seedStr[i-1]); 	LOG_SDEC("sendSeed _i_:",seedStr[i-1]);
	}
	//send seed
	DEBUG_MSG("sendSeed s");
	uart_send(2,seedStr);

	free(seedStr);

	LOG_SDEC("ended sendSeed: ok",0);

	return 0;
}

char multiplayer_syncSeed(unsigned int*seed)
{
	DEBUG_MSG("multiplayer_syncSeed 1");
	char received=1;//only for client, verifies if seed has been received
	char result = 1;
	switch (MULTIPLAYER_USER_TYPE)
	{
	case USER_TYPE_HOST:
		DEBUG_MSG("multiplayer_syncSeed h");
		result = sendSeed(seed);
		break;

	case USER_TYPE_CLIENT:
		DEBUG_MSG("multiplayer_syncSeed c1");
		 received = multiplayer_receive();

		 DEBUG_MSG("multiplayer_syncSeed c2");
		 //timeout
		 if (timeout_counter>=HOST_TIMEOUT && received!=OK) return 2;

		 DEBUG_MSG("multiplayer_syncSeed c3");
		 return received;

		break;
	default: return -1;
	}
	DEBUG_MSG("multiplayer_syncSeed e");
	return 0;
}

#endif //region:seed sync funcs


void process_received_inGameData()
{
	//use only boat pos for now

	timeout_counter=0;

	//try to fix possible error reading data
	//not 100% correct but will work most of the time due to probabilities
	if (numberOfBytesReceived>5)
	{
		short i=0;
		for (;i<numberOfBytesReceived-3;i++)
			if (received_info[i]==received_info[i+1])received_info[i+1]=received_info[i+2];
	}

	//update game stuff
	(*game)->multiplayerBoat.sp->x=(( (int) received_info[1]>>4)<<8)   + received_info[2];
	(*game)->multiplayerBoat.sp->y=(( (int) received_info[1]&0x0F)<<8 )+ received_info[3];
	//clean received_info
	multiplayer_reset_receivedInfo();
}


char multiplayer_send_inGameInfo()
{
	unsigned char* info2Send; ALLOC(unsigned char,3,info2Send);

	info2Send[0] = (unsigned char ) (
			(( ( (*game)->playerBoat.sp->x) & 0x0F00 ) >> 4 )+
			(( ((*game)->playerBoat.sp->y) >> 8 ) & 0x000F)
			);
	info2Send[1] = (*game)->playerBoat.sp->x & 0x00FF;
	info2Send[2] = (*game)->playerBoat.sp->y & 0x00FF;

	//send seed
	DEBUG_MSG("sendSeed s");
	uart_send(3,info2Send);

	free(info2Send);

	return 0;
}

char multiplayer_update()
{
	if (MULTIPLAYER_USER_TYPE!=USER_TYPE_NONE){
		if (sync_state==MULTIPLAYER_SYNC_RECEIVE)
			if (multiplayer_receive()==OK)
				{
					process_received_inGameData();
					sync_state=MULTIPLAYER_SYNC_SEND;
				}

		//send immediately after receiving an info "pack" if possible
		if (sync_state==MULTIPLAYER_SYNC_SEND)
			if (multiplayer_send_inGameInfo()==OK)
				sync_state=MULTIPLAYER_SYNC_RECEIVE;
	}
	/*else return 0;
	return 1;*/
	return 0;
}

char multiplayer_syncGameInit()
{
	if (MULTIPLAYER_USER_TYPE!=USER_TYPE_NONE){

		uart_send(0,0); //send start and end sign only
		if (multiplayer_receive()==OK)
		{
			multiplayer_reset_receivedInfo();
			return 0;
		}

	} else return 0;

	return 1;
}


/** @} end of multiplayerSync */
