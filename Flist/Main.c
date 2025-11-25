//========== DEFINE ==========
#define _CRT_SECURE_NO_WARNINGS

//========== HEADER ==========
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ui.h>

//========== STRUCT ==========
typedef struct new_node {
    struct new_node* prev;
    struct new_node* next;

    //flight information
    char airlines[3]; //IATA
    int flight_number;
    char depart_airport[4]; //IATA
    int depart_time;
    char arrive_airport[4]; //IATA
}new_node;

//========== FUNCTION ==========
new_node* add_flight(new_node* head, char airlines[3], int f_num, char depart_a[4], int depart_t, char arrive_a[4]);
new_node* delete_flight(new_node* head, char airlines[3], int f_num);
void upper(char* str);
int airport_check(char* airport, int locate);
void onAddClicked(uiButton* b, void* data);
void onDeleteClicked(uiButton* b, void* data);
void refresh_list_ui(uiMultilineEntry* list);
void onHelpButtonClicked(uiButton* b, void* data);

//========== GLOBAL VARIABLE ==========
	new_node* Head;
	uiWindow* main_window_global;



//==============================================
//                     CODE
//==============================================

//========== MAIN ==========
int main(void) {
	
    uiInitOptions options;
    const char* err;

	//LibUI 초기화
    memset(&options, 0, sizeof(options));
    err = uiInit(&options);
    if (err != NULL) {
        fprintf(stderr, "LibUI init error: %s\n", err);
        return 1;
    }

	//메인 창
    uiWindow* main_window = uiNewWindow("Flist", 400, 600, 1);
    uiWindowSetMargined(main_window, 1);
	main_window_global = main_window;

	uiBox* main_ver_box = uiNewVerticalBox();
	uiBoxSetPadded(main_ver_box, 1);
	uiWindowSetChild(main_window, uiControl(main_ver_box));
	


	//입력 방법 버튼
    uiButton* help_input = uiNewButton(u8"입력 방법");
    uiBoxAppend(main_ver_box, uiControl(help_input), 0);
    uiButtonOnClicked(help_input, onHelpButtonClicked, main_window);

	uiBox* mode_hor_box = uiNewHorizontalBox();
	uiBoxSetPadded(mode_hor_box, 1);
	uiBoxAppend(main_ver_box, uiControl(mode_hor_box), 0);

	

	//항공편 추가
	uiGroup* add_flight = uiNewGroup(u8"항공편 추가");
	uiBoxAppend(mode_hor_box, uiControl(add_flight), 1);

	uiForm* add_flight_form = uiNewForm();
	uiFormSetPadded(add_flight_form, 1);
	uiGroupSetChild(add_flight, uiControl(add_flight_form));

	uiEntry* add_airlines_entry = uiNewEntry();
	uiEntry* add_flight_number_entry = uiNewEntry();
	uiEntry* add_depart_airport_entry = uiNewEntry();
	uiEntry* add_depart_time_entry = uiNewEntry();
	uiEntry* add_arrive_airport_entry = uiNewEntry();
	uiButton* add_flight_button = uiNewButton(u8"추가");

	uiFormAppend(add_flight_form, u8"항공사", uiControl(add_airlines_entry), 0);
	uiFormAppend(add_flight_form, u8"편명", uiControl(add_flight_number_entry), 0);
	uiFormAppend(add_flight_form, u8"출발지", uiControl(add_depart_airport_entry), 0);
	uiFormAppend(add_flight_form, u8"출발 시간", uiControl(add_depart_time_entry), 0);
	uiFormAppend(add_flight_form, u8"도착지", uiControl(add_arrive_airport_entry), 0);
	uiFormAppend(add_flight_form, "", uiControl(add_flight_button), 0);



	//항공편 삭제
	uiGroup* delete_flight = uiNewGroup(u8"항공편 삭제");
	uiBoxAppend(mode_hor_box, uiControl(delete_flight), 1);

	uiForm* delete_flight_form = uiNewForm();
	uiFormSetPadded(delete_flight_form, 1);
	uiGroupSetChild(delete_flight, uiControl(delete_flight_form));

	uiEntry* delete_airlines_entry = uiNewEntry();
	uiEntry* delete_flight_number_entry = uiNewEntry();
	uiButton* delete_flight_button = uiNewButton(u8"삭제");

	uiFormAppend(delete_flight_form, u8"항공사", uiControl(delete_airlines_entry), 0);
	uiFormAppend(delete_flight_form, u8"편명", uiControl(delete_flight_number_entry), 0);
	uiFormAppend(delete_flight_form, "", uiControl(delete_flight_button), 0);



	//항공편 목록
	uiBox* flight_list_text = uiNewHorizontalBox();
	uiBoxSetPadded(flight_list_text, 1);

	uiLabel* flight = uiNewLabel(u8"항공편명");
	uiLabel* depart_airpot = uiNewLabel(u8"출발지");
	uiLabel* depart_time = uiNewLabel(u8"출발 시간");
	uiLabel* arrive_airpot = uiNewLabel(u8"도착지");

	uiBoxAppend(flight_list_text, uiControl(flight), 1);
	uiBoxAppend(flight_list_text, uiControl(depart_airpot), 1);
	uiBoxAppend(flight_list_text, uiControl(depart_time), 1);
	uiBoxAppend(flight_list_text, uiControl(arrive_airpot), 1);

	uiBoxAppend(main_ver_box, uiControl(flight_list_text), 0);

	uiMultilineEntry* flight_list = uiNewNonWrappingMultilineEntry();
	uiMultilineEntrySetReadOnly(flight_list, 1);
	uiBoxAppend(main_ver_box, uiControl(flight_list), 1);


    
	//버튼 작동
	uiMultilineEntry* add_data[6] = {
		(uiMultilineEntry*)add_airlines_entry,
		(uiMultilineEntry*)add_flight_number_entry,
		(uiMultilineEntry*)add_depart_airport_entry,
		(uiMultilineEntry*)add_depart_time_entry,
		(uiMultilineEntry*)add_arrive_airport_entry,
		flight_list
	};
	
	uiButtonOnClicked(add_flight_button, onAddClicked, add_data);

	uiMultilineEntry* delete_data[3] = {
		(uiMultilineEntry*)delete_airlines_entry,
		(uiMultilineEntry*)delete_flight_number_entry,
		flight_list
	};

	uiButtonOnClicked(delete_flight_button, onDeleteClicked, delete_data);

    uiControlShow(uiControl(main_window));
    uiMain();

	uiUninit();



    return 0;
}

//========== FUNCTION ==========

new_node* add_flight(new_node* head, char airlines[3], int f_num, char depart_a[4], int depart_t, char arrive_a[4])
{
	new_node* node = (new_node*)malloc(sizeof(new_node));

	strcpy(node->airlines, airlines);
	node->flight_number = f_num;
	strcpy(node->depart_airport, depart_a);
	node->depart_time = depart_t;
	strcpy(node->arrive_airport, arrive_a);

	node->next = NULL;
	node->prev = NULL;

	new_node* now_node = head;

	if (head == NULL) //리스트가 비어 있을 때 head삽입
	{
		return node;
	}

	if (depart_t < now_node->depart_time) //head 삽입
	{
		node->next = now_node;

		now_node->prev = node;

		return node;
	}

	while ((now_node->next != NULL) && (now_node->depart_time <= depart_t)) //출발 시간 순으로 항공편 위치 검사
	{
		now_node = now_node->next;
	}

	if ((now_node->next == NULL) && (now_node->depart_time <= depart_t)) //tail 삽입
	{
		node->prev = now_node;

		now_node->next = node;

		return head;
	}

	//중간 삽입
	node->next = now_node;
	node->prev = now_node->prev;

	now_node->prev->next = node;
	now_node->prev = node;

	return head;

}

new_node* delete_flight(new_node* head, char airlines[3], int f_num)
{
	new_node* now_node = head;

	while ((now_node != NULL) && ((strcmp(now_node->airlines, airlines) != 0) || (now_node->flight_number != f_num))) //일치하는 항공편 검사
	{
		now_node = now_node->next;
	}

	if (now_node == NULL) //일치하는 항공편이 없을때
	{
		return head;
	}

	if (now_node == head) //head 삭제
	{
		head = now_node->next;
		if (head != NULL)
		{
			head->prev = NULL;
		}

		free(now_node);

		return head;
	}

	if (now_node->next == NULL) //tail 삭제
	{
		now_node->prev->next = NULL;

		free(now_node);

		return head;
	}

	//중간 삭제
	now_node->prev->next = now_node->next;
	now_node->next->prev = now_node->prev;

	free(now_node);

	return head;
}

void upper(char* str) //소문자를 대문자로 변환
{
	for (int i = 0; i < strlen(str); i++)
	{
		if ((str[i] >= 'a') && (str[i] <= 'z'))
		{
			str[i] -= 32;
		}
	}
}

int airport_check(char* airport, int locate) //공항 코드 검사
{
	char airport_loacate[10];
	char error_message[64];
	(locate == 0) ? strcpy(airport_loacate, u8"출발지") : strcpy(airport_loacate, u8"도착지");
	sprintf(error_message, u8"%s 공항 코드가 잘못되었습니다.", airport_loacate);

	if (strlen(airport) != 3) //길이 검사
	{
		uiMsgBoxError(main_window_global, u8"오류", error_message);
		return 0;
	}
	for (int i = 0; i < strlen(airport); i++) //숫자 포함 검사
	{
		if ((48 <= airport[i]) && (airport[i] <= 57))
		{
			uiMsgBoxError(main_window_global, u8"오류", error_message);
			return 0;
		}
	}

	return 1;
}



//========== ADD BUTTON CALLBACK ==========
void onAddClicked(uiButton* b, void* data)
{
	uiMultilineEntry** entries = (uiMultilineEntry**)data;

	uiEntry* airlines_entry = (uiEntry*)entries[0];
	uiEntry* flightnum_entry = (uiEntry*)entries[1];
	uiEntry* departairport_entry = (uiEntry*)entries[2];
	uiEntry* departtime_entry = (uiEntry*)entries[3];
	uiEntry* arriveairport_entry = (uiEntry*)entries[4];
	uiMultilineEntry* flight_list = (uiMultilineEntry*)entries[5];

	new_node* now_node = Head;
	char* check_airlines = uiEntryText(airlines_entry);
	int check_flight_number = atoi(uiEntryText(flightnum_entry));
	char* check_depart_airport = uiEntryText(departairport_entry);
	int check_depart_time = atoi(uiEntryText(departtime_entry));
	char* check_arrive_airport = uiEntryText(arriveairport_entry);

	upper(check_airlines);
	upper(check_depart_airport);
	upper(check_arrive_airport);

	if (strlen(check_airlines) != 2) //항공사 코드 길이 검사
	{
		uiMsgBoxError(main_window_global, u8"오류", u8"항공사 코드가 잘못되었습니다.");
		return;
	}

	if ((airport_check(check_depart_airport, 0) == 0) || (airport_check(check_arrive_airport, 1) == 0)) //공항 코드 검사
	{
		return;
	}
	
	while (now_node != NULL) //중복 항공편 검사
	{
		if ((strcmp(now_node->airlines, check_airlines) == 0) && (now_node->flight_number == check_flight_number))
		{
			uiMsgBoxError(main_window_global, u8"오류", u8"이미 동일한 항공편이 존재합니다.");
			return;
		}
		now_node = now_node->next;
	}

	if (((check_depart_time / 100) > 23) || ((check_depart_time % 100) > 60)) //입력 시간 양식 검사
	{
		uiMsgBoxError(main_window_global, u8"오류", u8"시간 입력이 잘못되었습니다.");
		return;
	}



	char airlines[3];
	int flight_number;
	char depart_airport[4];
	int depart_time;
	char arrive_airport[4];

	strcpy(airlines, uiEntryText(airlines_entry));
	strcpy(depart_airport, uiEntryText(departairport_entry));
	strcpy(arrive_airport, uiEntryText(arriveairport_entry));

	flight_number = atoi(uiEntryText(flightnum_entry));
	depart_time = atoi(uiEntryText(departtime_entry));

	Head = add_flight(Head, airlines, flight_number, depart_airport, depart_time, arrive_airport);

	refresh_list_ui(flight_list);

	uiEntrySetText(airlines_entry, "");
	uiEntrySetText(flightnum_entry, "");
	uiEntrySetText(departairport_entry, "");
	uiEntrySetText(departtime_entry, "");
	uiEntrySetText(arriveairport_entry, "");
}

//========== DELETE BUTTON CALLBACK ==========
void onDeleteClicked(uiButton* b, void* data)
{
	uiMultilineEntry** entries = (uiMultilineEntry**)data;

	uiEntry* airlines_entry = (uiEntry*)entries[0];
	uiEntry* flightnum_entry = (uiEntry*)entries[1];
	uiMultilineEntry* flight_list = (uiMultilineEntry*)entries[2];

	char* check_airlines = uiEntryText(airlines_entry);
	int check_flight_number = atoi(uiEntryText(flightnum_entry));

	upper(check_airlines);

	if (strlen(check_airlines) != 2) //항공사 코드 길이 검사
	{
		uiMsgBoxError(main_window_global, u8"오류", u8"항공사 코드가 잘못되었습니다.");
		return;
	}

	char airlines[3];
	int flight_number;

	strcpy(airlines, uiEntryText(airlines_entry));
	flight_number = atoi(uiEntryText(flightnum_entry));

	Head = delete_flight(Head, airlines, flight_number);

	refresh_list_ui(flight_list);

	uiEntrySetText(airlines_entry, "");
	uiEntrySetText(flightnum_entry, "");
}

//========== LIST REFRESH ==========
void refresh_list_ui(uiMultilineEntry* list)
{
	uiMultilineEntrySetText(list, "");

	new_node* now_node = Head;
	char temp[256];

	while (now_node != NULL)
	{
		sprintf(temp, u8"%s%d  %25s %02d시 %02d분  %25s\n",
			now_node->airlines,
			now_node->flight_number,
			now_node->depart_airport,
			(now_node->depart_time / 100),
			(now_node->depart_time % 100),
			now_node->arrive_airport
		);

		uiMultilineEntryAppend(list, temp);
		now_node = now_node->next;
	}
}

//========== HELP BUTTON CALLBACK ==========
void onHelpButtonClicked(uiButton* b, void* data)
{
	uiWindow* parent = (uiWindow*)data;
	uiMsgBox(parent,
		u8"입력 방법",
		u8"항공사/공항: IATA 코드 (예: 대한항공 -> KE, 김포공항 -> GMP)\n"
		u8"편명: 숫자\n"
		u8"출발 시간: HHMM (예: 오후 3시 30분 -> 1530)\n"
	);
}