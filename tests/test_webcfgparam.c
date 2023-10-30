 /**
  * Copyright 2019 Comcast Cable Communications Management, LLC
  *
  * Licensed under the Apache License, Version 2.0 (the "License");
  * you may not use this file except in compliance with the License.
  * You may obtain a copy of the License at
  *
  *     http://www.apache.org/licenses/LICENSE-2.0
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
 */
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <CUnit/Basic.h>
#include <msgpack.h>
#include "../src/webcfg_param.h"
#define WEB_CFG_FILE "../../tests/wanmanager.bin"

int process_webcfgparam( webcfgparam_t *pm, msgpack_object *obj );

int readFromFile(char *filename,char **data, int *len)
{
	FILE *fp;
	int ch_count = 0;
	fp = fopen(filename, "r+");
	if (fp == NULL)
	{
		printf("Failed to open file %s. provide the file\n", WEB_CFG_FILE);
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	ch_count = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	*data = (char *) malloc(sizeof(char) * (ch_count + 1));
	fread(*data, 1, ch_count,fp);
        
	*len = ch_count;
	(*data)[ch_count] ='\0';
	fclose(fp);
	return 1;
}

void test_basic()
{

	webcfgparam_t *pm = NULL;
	int err, len=0, i=0;

	char *binfileData = NULL;
	int status = -1;
	printf("readFromFile\n");
	status = readFromFile(WEB_CFG_FILE,&binfileData , &len);

	printf("read status %d\n", status);
	if(status)
	{
		void* basicV;

		basicV = ( void*)binfileData;

		pm = webcfgparam_convert( basicV, len+1 );
		err = errno;
		printf( "errno: %s\n", webcfgparam_strerror(err) );		

		CU_ASSERT_FATAL( NULL != pm );
		CU_ASSERT_FATAL( NULL != pm->entries );
		CU_ASSERT_FATAL( 2 == pm->entries_count );
		CU_ASSERT_STRING_EQUAL( "Device.DeviceInfo.X_RDKCENTRAL-COM_CloudUIEnable", pm->entries[0].name );
		CU_ASSERT_FATAL( 3 == pm->entries[0].type );
		for(i = 0; i < (int)pm->entries_count ; i++)
		{
			printf("pm->entries[%d].name %s\n", i, pm->entries[i].name);
			printf("pm->entries[%d].value %s\n" , i, pm->entries[i].value);
			printf("pm->entries[%d].type %d\n", i, pm->entries[i].type);
		}

	    	webcfgparam_destroy( pm );
	}
}

void test_webcfgparam_strerror()
{
	const char *txt;
	txt = webcfgparam_strerror(0);
	CU_ASSERT_STRING_EQUAL(txt,"No errors.");
	txt = webcfgparam_strerror(1);
	CU_ASSERT_STRING_EQUAL(txt,"Out of memory.");	
	txt = webcfgparam_strerror(10);
	CU_ASSERT_STRING_EQUAL(txt,"Unknown error.");	
}

void test_process_webcfgparam()
{
	msgpack_object *inner;
	webcfgparam_t *pm;
	int ret=0,err=0;
	inner = malloc( sizeof(msgpack_object));
	pm = malloc( sizeof(webcfgparam_t));
	inner->via.array.size=0;
	ret = process_webcfgparam(pm,inner);
        CU_ASSERT_EQUAL(ret, 0);

	inner->via.array.size=1;
	msgpack_object_array *array = &inner->via.array;
	array->ptr = malloc( sizeof(msgpack_object));
	array->ptr->type = 0;
	ret = process_webcfgparam(pm,inner);
        CU_ASSERT_EQUAL(ret, -1);
	err = errno;
	const char *txt;
	txt = webcfgparam_strerror(err);
	printf( "errno: %s\n", txt );
	CU_ASSERT_STRING_EQUAL(txt,"Invalid 'parameters' array.");
}

void test_process_params_invalid_datatype()
{
	int err, len=0;
	char *binfileData = NULL;
	int status = -1;
	printf("readFromFile\n");
	status = readFromFile("../../tests/test1.bin",&binfileData , &len);

	printf("read status %d\n", status);
	if(status)
	{
		void* basicV;

		basicV = ( void*)binfileData;

		webcfgparam_convert( basicV, len+1 );
		err = errno;
		const char *txt;
		txt = webcfgparam_strerror(err);	
		CU_ASSERT_STRING_EQUAL(txt,"Invalid 'blob' object.");					
	}
}

void add_suites( CU_pSuite *suite )
{
    *suite = CU_add_suite( "tests", NULL, NULL );
    CU_add_test( *suite, "Full", test_basic);
	CU_add_test( *suite, "test webcfgparam_strerror", test_webcfgparam_strerror);
	CU_add_test( *suite, "test process_webcfgparam", test_process_webcfgparam);	
    CU_add_test( *suite, "test process_params", test_process_params_invalid_datatype);	
}

/*----------------------------------------------------------------------------*/
/*                             External Functions                             */
/*----------------------------------------------------------------------------*/
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

