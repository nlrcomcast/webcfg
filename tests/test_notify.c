#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include "../src/webcfg_notify.h"
void free_notify_params_struct(notify_params_t *param);
#define UNUSED(x) (void )(x)
bool shutdown_flag = false;
char *get_deviceMAC(void)
{
	return "123456789000";
}

void sendNotification_rbus(char *payload, char *source, char *destination)
{
    UNUSED(payload);
    UNUSED(source);
    UNUSED(destination);
}

void sendNotification(char *payload, char *source, char *destination)
{
	WebcfgDebug("B4 sendNotification_rbus\n");
	sendNotification_rbus(payload, source, destination);
}

bool get_global_shutdown()
{
	return shutdown_flag;
}

void test_getStatusErrorCodeAndMessage()
{
    char* result = NULL;
    uint16_t ret = 0;
    ret = getStatusErrorCodeAndMessage(DECODE_ROOT_FAILURE,&result);
    CU_ASSERT_EQUAL(ret, 111);
    CU_ASSERT_STRING_EQUAL(result, "decode_root_failure");  

    ret = getStatusErrorCodeAndMessage(INCORRECT_BLOB_TYPE,&result);
    CU_ASSERT_EQUAL(ret, 211);
    CU_ASSERT_STRING_EQUAL(result, "incorrect_blob_type");    

    ret = getStatusErrorCodeAndMessage(BLOB_PARAM_VALIDATION_FAILURE,&result);
    CU_ASSERT_EQUAL(ret, 211);
    CU_ASSERT_STRING_EQUAL(result, "blob_param_validation_failure");       

    ret = getStatusErrorCodeAndMessage(WEBCONFIG_DATA_EMPTY,&result);
    CU_ASSERT_EQUAL(ret, 211);
    CU_ASSERT_STRING_EQUAL(result, "webconfig_data_empty");  

    ret = getStatusErrorCodeAndMessage(MULTIPART_BOUNDARY_NULL,&result);
    CU_ASSERT_EQUAL(ret, 211);
    CU_ASSERT_STRING_EQUAL(result, "multipart_boundary_NULL");    

    ret = getStatusErrorCodeAndMessage(INVALID_CONTENT_TYPE,&result);
    CU_ASSERT_EQUAL(ret, 211);
    CU_ASSERT_STRING_EQUAL(result, "invalid_content_type");     

    ret = getStatusErrorCodeAndMessage(ADD_TO_CACHE_LIST_FAILURE,&result);
    CU_ASSERT_EQUAL(ret, 311);
    CU_ASSERT_STRING_EQUAL(result, "add_to_cache_list_failure");  

    ret = getStatusErrorCodeAndMessage(FAILED_TO_SET_BLOB,&result);
    CU_ASSERT_EQUAL(ret, 311);
    CU_ASSERT_STRING_EQUAL(result, "failed_to_set_blob");    

    ret = getStatusErrorCodeAndMessage(MULTIPART_CACHE_NULL,&result);
    CU_ASSERT_EQUAL(ret, 311);
    CU_ASSERT_STRING_EQUAL(result, "multipart_cache_NULL");       

    ret = getStatusErrorCodeAndMessage(AKER_SUBDOC_PROCESSING_FAILED,&result);
    CU_ASSERT_EQUAL(ret, 411);
    CU_ASSERT_STRING_EQUAL(result, "aker_subdoc_processing_failed");  

    ret = getStatusErrorCodeAndMessage(AKER_RESPONSE_PARSE_FAILURE,&result);
    CU_ASSERT_EQUAL(ret, 411);
    CU_ASSERT_STRING_EQUAL(result, "aker_response_parse_failure");    

    ret = getStatusErrorCodeAndMessage(INVALID_AKER_RESPONSE,&result);
    CU_ASSERT_EQUAL(ret, 411);
    CU_ASSERT_STRING_EQUAL(result, "invalid_aker_response");        

    ret = getStatusErrorCodeAndMessage(LIBPARODUS_RECEIVE_FAILURE,&result);
    CU_ASSERT_EQUAL(ret, 411);
    CU_ASSERT_STRING_EQUAL(result, "libparodus_receive_failure"); 
 
    ret = getStatusErrorCodeAndMessage(COMPONENT_EVENT_PARSE_FAILURE,&result);
    CU_ASSERT_EQUAL(ret, 511);
    CU_ASSERT_STRING_EQUAL(result, "component_event_parse_failure"); 

    ret = getStatusErrorCodeAndMessage(SUBDOC_RETRY_FAILED,&result);
    CU_ASSERT_EQUAL(ret, 611);
    CU_ASSERT_STRING_EQUAL(result, "subdoc_retry_failed"); 
 
    ret = getStatusErrorCodeAndMessage(999,&result);
    CU_ASSERT_EQUAL(ret, 0);
    CU_ASSERT_STRING_EQUAL(result, "Unknown Error");               
}

void test_addWebConfgNotifyMsg()
{
addWebConfgNotifyMsg("moca",123,"xyz","none","1122334455",10,"req",400,"root",200);
addWebConfgNotifyMsg("moca",0,"xyz","none","1122334455",10,"req",400,"root",404);
}

void test_initWebConfigNotifyTask()
{
    initWebConfigNotifyTask();
}

void test_free_notify_params_struct()
{
	notify_params_t *msg = NULL;
	msg = (notify_params_t *)malloc(sizeof(notify_params_t));
	memset(msg, 0, sizeof(notify_params_t));    
    msg->name = strdup("test1");
    // msg->application_status = strdup("test2");
    // msg->error_details = strdup("test3");
    // msg->version = strdup("test4");
    // msg->transaction_uuid = strdup("test5");
    // msg->type = strdup("test6");  
    free_notify_params_struct(msg);
    CU_ASSERT_EQUAL(msg->name, NULL);     
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "tests", NULL, NULL );
    CU_add_test( *suite, "test getStatusErrorCodeAndMessage", test_getStatusErrorCodeAndMessage);
    CU_add_test( *suite, "test free_notify_params_struct", test_free_notify_params_struct);        
    CU_add_test( *suite, "test addWebConfgNotifyMsg", test_addWebConfgNotifyMsg);
    CU_add_test( *suite, "test initWebConfigNotifyTask", test_initWebConfigNotifyTask);    
}

int main( int argc, char *argv[] )
{
    unsigned rv = 1;
    CU_pSuite suite = NULL;

    (void ) argc;
    (void ) argv;

    if( CUE_SUCCESS == CU_initialize_registry() ) {
        add_suites( &suite );

        if( NULL != suite ) {
            CU_basic_set_mode( CU_BRM_VERBOSE );
            CU_basic_run_tests();
            printf( "\n" );
            CU_basic_show_failures( CU_get_failure_list() );
            printf( "\n\n" );
            rv = CU_get_number_of_tests_failed();
        }

        CU_cleanup_registry();

    }

    return rv;
}
 