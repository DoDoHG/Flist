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
void onAddClicked(uiButton* b, void* data);
void onDeleteClicked(uiButton* b, void* data);
void refresh_list_ui(uiMultilineEntry* list);
void onHelpClicked(uiMenuItem* item, void* data);

//========== GLOBAL VARIABLE ==========
	new_node* Head;



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

    uiWindow* main_window = uiNewWindow("Flist", 400, 600, 1);
    uiWindowSetMargined(main_window, 1);

	uiMenu* menubar_help = uiNewMenu(u8"도움말");
	uiMenuItem* menubar_help_help = uiMenuAppendItem(menubar_help, u8"입력 방법");
	

	uiMenuItemOnClicked(menubar_help_help, onHelpClicked, main_window);

	
	
	


	uiBox* main_ver_box = uiNewVerticalBox();
	uiBoxSetPadded(main_ver_box, 1);
	uiWindowSetChild(main_window, uiControl(main_ver_box));
	


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

	while ((now_node->next != NULL) && (now_node->depart_time < depart_t)) //출발 시간 순으로 항공편 위치 검사
	{
		now_node = now_node->next;
	}

	if ((now_node->next == NULL) && (now_node->depart_time <= depart_t)) //tail 삽입
	{
		node->prev = now_node;

		now_node->next = node;

		return head;
	}

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


	now_node->prev->next = now_node->next;
	now_node->next->prev = now_node->prev;

	free(now_node);

	return head;
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

	char airlines[3];

	char depart[4];
	char arrive[4];
	int flightnum;
	int departtime;

	strcpy(airlines, uiEntryText(airlines_entry));
	strcpy(depart, uiEntryText(departairport_entry));
	strcpy(arrive, uiEntryText(arriveairport_entry));

	flightnum = atoi(uiEntryText(flightnum_entry));
	departtime = atoi(uiEntryText(departtime_entry));

	Head = add_flight(Head, airlines, flightnum, depart, departtime, arrive);

	refresh_list_ui(flight_list);
}

//========== DELETE BUTTON CALLBACK ==========
void onDeleteClicked(uiButton* b, void* data)
{
	uiMultilineEntry** entries = (uiMultilineEntry**)data;

	uiEntry* airlines_entry = (uiEntry*)entries[0];
	uiEntry* flightnum_entry = (uiEntry*)entries[1];
	uiMultilineEntry* flight_list = (uiMultilineEntry*)entries[2];

	char airlines[3];
	int flightnum;

	strcpy(airlines, uiEntryText(airlines_entry));
	flightnum = atoi(uiEntryText(flightnum_entry));

	Head = delete_flight(Head, airlines, flightnum);

	refresh_list_ui(flight_list);
}

//========== LIST REFRESH ==========
void refresh_list_ui(uiMultilineEntry* list)
{
	uiMultilineEntrySetText(list, "");

	new_node* now_node = Head;
	char temp[256];

	while (now_node != NULL)
	{
		sprintf(temp, "%s %d  %s -> %s  %d\n",
			now_node->airlines,
			now_node->flight_number,
			now_node->depart_airport,
			now_node->arrive_airport,
			now_node->depart_time
		);

		uiMultilineEntryAppend(list, temp);
		now_node = now_node->next;
	}
}

void onHelpClicked(uiMenuItem* item, void* data)
{
	uiMsgBox(data, u8"도움말", u8"항공사 및 공항 입력시 IATA 코드를 사용해주세요.\n(예시: 대한항공: KE)\n\n출발 시각은 24시간 형식을 사용해주세요.\n(예: 오후4시 20분 -> 1620");
}