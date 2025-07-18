//=====[#include guards - begin]===============================================

#ifndef _EVENT_LOG_H_
#define _EVENT_LOG_H_
#include <stdbool.h>
//=====[Declaration of public defines]=========================================
extern bool displayLogConfirmation;
extern int displayLogConfirmationTimerMs;

#define LOG_CONFIRMATION_DISPLAY_TIME_MS 2000
#define EVENT_LOG_MAX_STORAGE        20
#define EVENT_HEAD_STR_LENGTH         8
#define EVENT_LOG_NAME_MAX_LENGTH    13
#define DATE_AND_TIME_STR_LENGTH     18
#define CTIME_STR_LENGTH             25
#define NEW_LINE_STR_LENGTH           3
#define EVENT_STR_LENGTH             (EVENT_HEAD_STR_LENGTH + \
                                      EVENT_LOG_NAME_MAX_LENGTH + \
                                      DATE_AND_TIME_STR_LENGTH  + \
                                      CTIME_STR_LENGTH + \
                                      NEW_LINE_STR_LENGTH)

//=====[Declaration of public data types]======================================

//=====[Declarations (prototypes) of public functions]=========================

void eventLogUpdate();
int eventLogNumberOfStoredEvents();
void eventLogRead( int index, char* str );
void eventLogWrite( bool currentState, const char* elementName );
bool eventLogSaveToSdCard();

//=====[#include guards - end]=================================================

#endif // _EVENT_LOG_H_
