#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

//코드 비정상으로 종료되었을 때 "taskkill /im 테스트중입니다.exe /f" 명령어를 실행하고 실행해주면 실행됨

//프로토타입
void addStudent(int key, int grade, char *name, char *charge, char *event);
void deleteStudent(int key);
void printList();
void use_modify(int key);
void gradeList();
void keyList();
void diverseList(int method);
void event_modify(int key, FILE* fp,int *num);
int CheckStudentNumber(int stdnum);
void SortbyKey(int key, int grade, char* name, char* charge, char* event);
void Sortbygrade(int key, int grade, char* name, char* charge, char* event);
void moneyAdd(int date,int income, int outcome,int toal, char* text);
void moneyWrite(FILE* fp, FILE* fp2, int date, int income, int outcome, int total, char* text);
int smallLetter(char* result);//참석여부 o,x 확인

struct node {
	int key; //학번
	int grade;//학년
	char name[100];//최대 10자인 이름
	char charge[2];//학생회비여부 o,x
	char event[2];//행사 참석 여부 o,x
	struct node* next; //학번 순 정렬
};
struct node *head;//첫 노드가 들어간다.
struct node* grade_head;//학년순 노드
struct node* key_head;//학번순 노드

struct money{
	int date;//날짜
	int income; //입금
	int outcome;//출금
	int total;//잔액
	char text[100];//내역
	struct money* next;
};
struct money* headMoney;

void main() {
	char file_name[100];//행사관리 파일이름으로 쓰일 예정
	int key = 0; //학번
	int grade = 0;//학년
	char name[100];//최대 10자인 이름
	char charge[2];//학생회비여부 o,x
	char event[2] = { "x" };//행사 참석 여부 o,x,디폴트값
	int num = 0;//참석자 수
	FILE* fp = NULL;
	FILE* fp2 = NULL;
	//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//Dipensive code-학생리스트
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	fopen_s(&fp,"studentList.txt", "r");
	if (fp == NULL) {
		printf("파일을 찾을 수가 없습니다.\n");
		return;
	}
	//모든 학생 struct에 저장(linked list로 학번순)
	while (fscanf_s(fp, "%d %d %s %s %s", &(key), &(grade), name,100, charge,2, event,2)==5) {
		addStudent(key, grade, name, charge, event);
	}
	fclose(fp);
	
	//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//Dipensive code-회비내역리스트
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	//회비내역 작성 전 셋팅
	int date;
	int income = 0;
	int outcome = 0;
	int total = 0;
	char text[100];

	fopen_s(&fp,"학생회비내역.txt", "r");
	if (fp == NULL) {
		printf("파일을 찾을 수가 없습니다.\n");
		return;
	}
	//모든 회비내역 struct에 저장(날짜순)
	while (fscanf_s(fp, "%d %d %d %d %s", &date, &income,&outcome, &total, text,100) == 5) {
		moneyAdd(date, income, outcome, total, text);
	}
	fclose(fp);

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	int program = 100;//프로그램 선택 스위치
	int process = 100;//프로그램 안의 기능 선택 스위치

	while (1)
	{
		printf("안녕하세요 oo학과 관리자님 프로그램을 시작하겠습니다.\n\n");
		printf("\n\n*****주의 : 행사관리는 행사 시에만 신중히 선택해주세요*****\n");
		printf("어떤 프로그램을 원하세요? (0. 종료 1.학생관리 2.행사관리) : ");
		scanf_s("%d", &program);
		process = 100;//초기화
		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//학생관리 프로그램
		//-------------------------------------------------------------------------------------------------------------------------------------------------

		if (program == 1) {
			printf("---------------------------------------\n\n");
			printf("학생관리 프로그램을 시작하겠습니다.\n\n");

			while (process != 0)
			{
				printf("어떤 작업을 수행하시겠습니까?\n(0.종료/1.학생추가/2.학생삭제/3.학생정보수정/4.전체학생보기) : ");

				scanf_s("%d", &process);
				printf("\n");

				//학생추가
				if (process == 1) {

					printf("추가할 학생의 학번을 입력해주세요. :");
					scanf_s("%d", &key);

					//이미있는 학번인지 체크
					if (CheckStudentNumber(key) == 1) {
						continue;
					}


					printf("추가할 학생의 학년을 입력해주세요. :");
					scanf_s("%d", &grade);

					printf("추가할 학생의 이름을 입력해주세요. :");
					scanf_s("%s", name, 100);

					printf("추가할 학생의 학생회비 여부를 입력해주세요.(소문자 o,x) :");
					scanf_s("%s", charge, 2);	

					while (1) {
						if (smallLetter(charge) == 1) {
							printf("수정할 학생회비 여부를 적어주세요.(소문자 o,x) : ");
							scanf_s("%s", charge, 2);
						}
						else {
							break;
						}

					}

					fopen_s(&fp,"studentList.txt", "a");
					fprintf(fp, "%d %d %s %s %s\n", key, grade,name, charge, event);//파일에추가(파일에는 학생정보 순서상관x)
					fclose(fp);

					addStudent(key, grade, name, charge, event);//노드 순서 처리
				}

				//학생삭제
				else if (process == 2) {

					while (1) {
						printf("삭제할 학생의 학번을 입력해주세요. :");
						scanf_s("%d", &key);
						if (CheckStudentNumber(key) == 0) {
							printf("해당 학생이 존재하지 않습니다.\n\n");
						}
						else {
							break;
						}
					}

					deleteStudent(key);
					//학생기록 삭제된 학생 빼고 다시 입력
					struct node *ptr = (struct node*)malloc(sizeof(struct node));
					ptr = head;

					fopen_s(&fp,"studentList.txt", "w");
					while (ptr != NULL) {
						fprintf(fp, "%d %d %s %s %s\n", ptr->key, ptr->grade, ptr->name, ptr->charge, ptr->event);
						ptr = ptr->next;
					}
					fclose(fp);
					free(ptr);
				}

				//학생정보 수정하기
				else if (process == 3) {
					//정보가 있는지 확인하기
					while (1) {
						printf("누구의 정보를 수정하시겠습니까?(학번을 입력해주세요) :");
						scanf_s("%d", &key);
						if (CheckStudentNumber(key) == 0) {
							printf("해당 학생이 존재하지 않습니다.\n\n");
						}
						else {
							break;
						}
					}

					use_modify(key);

					//수정된 정보로 다시 작성
					struct node* ptr = (struct node*)malloc(sizeof(struct node));
					ptr = head;
					fopen_s(&fp,"studentList.txt", "w");
					while (ptr != NULL) {
						fprintf(fp, "%d %d %s %s %s\n", ptr->key, ptr->grade, ptr->name, ptr->charge, ptr->event);
						ptr = ptr->next;
					}
					fclose(fp);
					free(ptr);
				}

				//전체 학생 출력
				else if (process == 4) {

					printList();
				}
				else if (process == 0) {
					printf("\n\n학생관리 프로그램을 종료하겠습니다.\n");
					printf("\n\n---------------------------------------\n\n");

					Sleep(3000);//3초 안내문을 보여주기위한 장치
					system("cls");//새로운 시작을 위한 화면 클리어
				}
				else {
					printf("잘못 된 입력 값 입니다. 다시 입력해주세요.\n\n");
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//행사관리 프로그램
		//-------------------------------------------------------------------------------------------------------------------------------------------------

		else if (program == 2) {
			int method = 0;//학생명단 여는 방식 스위치
			int selected = 100;//다른방식 or 참석여부 수정
			printf("---------------------------------------\n\n");
			printf("행사관리 프로그램을 시작하겠습니다.\n");
			printf("행사 파일명을 입력해주세요.(ex.간식행사.txt): ");
			scanf_s("%s", file_name, 100);
			printf("\n\n");

			fopen_s(&fp,file_name, "a");//읽기로 열면 파일이 없으면 생성안해줌 , 참석자 명단

			printf("학생명단을 어떤 방식으로 여시겠습니까?\n(1.학번별/2.학년별/3.전체보기): ");
			scanf_s("%d", &method);
			diverseList(method);

			while (selected != 0) {
				printf("\n어떤 작업을 수행하시겠습니까? (1.다른 방식의 명단/2.참석여부 수정/0.종료):");
				scanf_s("%d", &selected);
				if (selected == 1) {
					printf("\n어떤 방식으로 명단을 보시겠습니까?\n(1.학번별/2.학년별/3.전체보기) : ");
					scanf_s("%d", &method);
					printf("\n\n");
					diverseList(method);
				}
				else if (selected == 2) {
					while (1) {
						printf("\n학번을 입력해주시면 참석여부가 o로 변경됩니다.(0을 눌러주시면 수정이 종료됩니다.)\n: ");
						scanf_s("%d", &key);
						if (key == 0) {
							break;
						}
						event_modify(key, fp, &num);
						//행사참여정보는 원본에 수정하지 않는다 왜냐하면 참석자 명단이 따로 써지기도하고 다음 행사때 불편
					}
				}
				else if (selected == 0) {
					printf("\n\n행사관리 프로그램을 종료하겠습니다.\n");
					printf("---------------------------------------\n\n");
					fprintf(fp, "----------------------------------------------------------------------------------\n");
					fprintf(fp, "\n총 %d명의 학생이 참석하셨습니다.", num);
					printf("참여자 명단 파일작성이 완료되었습니다.\n\n");
					fclose(fp);

					int last_date;
					printf("행사 날짜는 언제입니까? (ex. 20190301) : ");
					scanf_s("%d", &date);

					
					struct money* ptr = (struct money*)malloc(sizeof(struct money));
					ptr=headMoney;
					//마지막 날짜
					while (ptr != NULL) {
						last_date = ptr->date;
						ptr = ptr->next;
					}
					free(ptr);

					//마지막 날짜보다 전 날짜면 다시 입력을 받는다. (total계산에 혼란을 주지 않기 위함)
					while (1) {
						if (date < last_date) {
							printf("행사 날짜가 마지막 날짜보다 전 날짜 입니다.\n\n");
							printf("행사 날짜는 언제입니까? (ex. 20190301) : ");
							scanf_s("%d", &date);
						}
						else {
							break;
						}
					}

					printf("행사 입금액은 얼마입니까? : ");
					scanf_s("%d", &income);


					printf("행사 출금액은 얼마입니까? : ");
					scanf_s("%d", &outcome);

					printf("내역을 작성해주세요. : ");
					scanf_s("%s", text, 100);

					moneyWrite(fp,fp2, date, income, outcome, total, text);

					printf("학생회비 정보를 저장했습니다.\n\n");

					Sleep(3000);//3초 안내문을 보여주기위한 장치
					system("cls");//새로운 시작을 위한 화면 클리어
				}
				else {
					printf("\n없는 작업입니다. 다시입력해주세요.\n\n");
				}
			}
		}

		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//프로그램 종료
		//-------------------------------------------------------------------------------------------------------------------------------------------------
		else if (program == 0) {
			printf("---------------------------------------\n\n");
			printf("프로그램을 종료하겠습니다.\n");
			break;
		}


		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//잘못 된 값 입력
		//-------------------------------------------------------------------------------------------------------------------------------------------------
		else {
			printf("잘못 된 입력 값 입니다. 다시 입력해주세요.\n\n");
		}

	}


	
}





//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//사용자 정의 함수
//-------------------------------------------------------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------------------------------------------------------
											//초기학생노드추가,학생관리프로그램 - 학생 추가함수
//-------------------------------------------------------------------------------------------------------------------------------------------------
void addStudent(int key, int grade, char* name, char* charge, char* event)
{

	struct node* newStudent = (struct node*)malloc(sizeof(struct node));
	//새 학생의 데이터추가
	newStudent->key = key;
	newStudent->grade = grade;
	strcpy_s(newStudent->name,100, name);//첫주소를 넘겨준다
	strcpy_s(newStudent->charge,2, charge);
	strcpy_s(newStudent->event,2, event);
	newStudent->next = NULL;
	
	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드(순차적으로 노드교체)
	struct node* prev_ptr = NULL;//내가 처리하고 있던 전 노드
	ptr = head;
	int flag = 0;//맨마지막에 들어갈지 아닐지를 결정해준다

	if (head == NULL) {
		head = newStudent;
		return;
	}
	while (ptr != NULL) {
		if (ptr->key > newStudent->key) {
			if (prev_ptr == NULL) {
				newStudent->next = ptr;
				head = newStudent;
			}
			else {
				newStudent->next = ptr;
				prev_ptr->next = newStudent;
			}
			flag = 1;
			break;
		}
		prev_ptr = ptr;
		ptr = ptr->next;
	}
	if (flag == 0) prev_ptr->next = newStudent;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//학생관리프로그램 - 학생삭제함수 (key는 학번)
//-------------------------------------------------------------------------------------------------------------------------------------------------
void deleteStudent(int key)
{
	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드
	struct node* prev_ptr = NULL; //내가 처리했던 이전의 노드
	ptr = head;

	while (ptr != NULL) {
		if (ptr->key == key) {
			//노드 첫 값을 지울 때
			if (ptr == head) {
				head = ptr->next;
			}
			else {
				prev_ptr->next = ptr->next;
				free(ptr);
			}
			break;
		}
		prev_ptr = ptr;
		ptr = ptr->next;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//학생관리프로그램 - 학생전체출력함수
//-------------------------------------------------------------------------------------------------------------------------------------------------
void printList()
{
	int num=0;
	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드
	ptr = head;
	printf("%8s%15s\t%20s\t%13s\t%10s\n","학번","이름","학년","학생회비여부","행사참여여부");
	printf("----------------------------------------------------------------------------------\n");
	
	while (ptr != NULL) {
		
		printf("%10d%15s\t%10d\t%10s\t%10s\n", ptr->key, ptr->name, ptr->grade, ptr->charge, ptr->event);
		num++;
		ptr = ptr->next;
	}
	printf("----------------------------------------------------------------------------------\n");
	printf("\n총 %d명의 학생이 검색되었습니다.\n\n", num);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//학생관리프로그램 - 사용자정보수정
//-------------------------------------------------------------------------------------------------------------------------------------------------
void use_modify(int key) {
	int modify = 100;//어떤 정보를 수정할건지 스위치
	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드

	ptr = head;

	
	//노드에서 같은 학번을 탐색
	
	while (ptr != NULL) {
		if (ptr->key == key) {
			printf("\n학생의 어떤 정보를 수정하시겠습니까?\n");
			printf("(1.학번/2.학년/3.이름/4.학생회비여부/0.수정종료): ");
			scanf_s("%d", &modify);
			if (modify == 1) {
				printf("수정할 학번을 적어주세요. : ");
				scanf_s("%d", &(ptr->key));
				printf("학번수정이 완료되었습니다.");
			}
			else if (modify == 2) {
				printf("수정할 학년을 적어주세요. : ");
				scanf_s("%d", &(ptr->grade));
				printf("학년수정이 완료되었습니다.\n");

			}
			else if (modify == 3) {
				printf("수정할 이름을 적어주세요. : ");
				scanf_s("%s", ptr->name, 100);
				printf("이름수정이 완료되었습니다.\n");

			}
			else if (modify == 4) {
				printf("수정할 학생회비여부를 적어주세요.(소문자 o,x) : ");
				scanf_s(" %s", ptr->charge, 2);
				//소문자확인
				while (1) {
					if (smallLetter(ptr->charge) == 1) {
						printf("수정할 학생회비여부를 적어주세요.(소문자 o,x) : ");
						scanf_s("%s", ptr->charge, 2);
					}
					else {
						break;
					}
				}
			}	
			else if (modify == 0) {
				printf("학생 수정을 마치겠습니다.\n\n");
				return;
			}
		}
		else{
			ptr = ptr->next;
		}
	}
	printf("찾으시는 학생의 정보가 없습니다.\n\n");
		
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
											//학생관리프로그램-소문자o,x확인
//-------------------------------------------------------------------------------------------------------------------------------------------------
int smallLetter(char* result) {
	if (*result == 'o'|| *result == 'x') {
		printf("\n완료되었습니다.\n\n");
		return 0;
	}
	else {
		printf("o 또는 x가 아닙니다 다시 입력해주세요.\n\n");
		return 1;
	}
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
											//행사관리프로그램 - 행사참석여부만 수정하기
//-------------------------------------------------------------------------------------------------------------------------------------------------
void event_modify(int key, FILE* fp, int* num) {
	int modify = 100;//어떤 정보를 수정할건지 스위치
	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드
	ptr = head;
	//노드에서 같은 학번을 탐색->참석여부수정
	while (ptr != NULL) {
		if (ptr->key == key) {
			strcpy_s(ptr->event,2, "o");
			printf("참석여부가 변경되었습니다.\n\n");
			fprintf(fp, "%d %s %d %s %s\n", ptr->key, ptr->name, ptr->grade, ptr->charge, ptr->event);
			(*num)++;
		}
		ptr = ptr->next;
	}
	if(ptr == NULL && *num==0) {
	printf("찾으시는 학생의 정보가 없습니다.\n");
	}

	
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
											//행사관리프로그램 - 학년별 명단출력
//-------------------------------------------------------------------------------------------------------------------------------------------------

void gradeList() {
	FILE* fp = NULL;
	int key = 0; //학번
	int grade = 0;//학년
	char name[100];//최대 10자인 이름
	char charge[2];//학생회비여부 o,x
	char event[2] = { "x" };//행사 참석 여부 o,x,디폴트값
	

	fopen_s(&fp, "studentList.txt", "r");
	if (fp == NULL) {
		printf("파일을 찾을 수가 없습니다.\n");
		return;
	}
	while (fscanf_s(fp, "%d %d %s %s %s", &(key), &(grade), name, 100, charge, 2, event, 2) == 5) {
		Sortbygrade(key, grade, name, charge, event);
	}

	//출력
	int count_grade = 1;//1학년부터시작
	int num = 0;
	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드
	ptr = grade_head;

	while (ptr != NULL) {
		printf(">>%d학년\n", count_grade);
		printf("%8s%15s\t%20s\t%13s\t%10s\n", "학년", "이름", "학번", "학생회비여부", "행사참여여부");
		printf("----------------------------------------------------------------------------------\n");
		while(ptr != NULL && ptr->grade == count_grade){
			printf("%10d%15s\t%10d\t%10s\t%10s\n", ptr->grade, ptr->name, ptr->key, ptr->charge, ptr->event);
			num++;
			ptr = ptr->next;
		}
		printf("----------------------------------------------------------------------------------\n");
		printf("\n총 %d명의 학생이 검색되었습니다.\n\n", num);
		num = 0;
		count_grade++;
	}
	fclose(fp);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------
											//행사관리프로그램 - 학번별 명단출력(2014~2019)
//-------------------------------------------------------------------------------------------------------------------------------------------------
void keyList() {
	FILE* fp = NULL;
	int key = 0; //학번
	int grade = 0;//학년
	char name[100];//최대 10자인 이름
	char charge[2];//학생회비여부 o,x
	char event[2] = { "x" };//행사 참석 여부 o,x,디폴트값


	fopen_s(&fp, "studentList.txt", "r");
	if (fp == NULL) {
		printf("파일을 찾을 수가 없습니다.\n");
		return;
	}
	while (fscanf_s(fp, "%d %d %s %s %s", &(key), &(grade), name, 100, charge, 2, event, 2) == 5) {
		SortbyKey(key, grade, name, charge, event);
	}

	//출력
	int count_key = 201400000;//1학년부터시작
	int num = 0;
	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드
	ptr = key_head;

	while (ptr != NULL) {
		printf(">>%d학번\n", count_key / 100000);
		printf("%8s%15s\t%20s\t%13s\t%10s\n", "학번", "이름", "학년", "학생회비여부", "행사참여여부");
		printf("----------------------------------------------------------------------------------\n");

		while (ptr != NULL&&ptr->key/100000==count_key/100000) {
			printf("%10d%15s\t%10d\t%10s\t%10s\n", ptr->key, ptr->name, ptr->grade, ptr->charge, ptr->event);
			num++;
			ptr = ptr->next;
		}
		printf("----------------------------------------------------------------------------------\n");
		printf("\n총 %d명의 학생이 검색되었습니다.\n\n", num);
		num = 0;
		count_key+=100000;
	}
	fclose(fp);
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//행사관리프로그램 - 다양한 방식의 명단
//-------------------------------------------------------------------------------------------------------------------------------------------------
void diverseList(int method) {
	int j = 1;//학년 카운트
	struct node* ptr;   // 내가 처리하고(가리키고) 있는 노드
	ptr = head;

	//학년별 명단
	if (method == 1) {
		printf("\n>>다음은 학번별 명단입니다.\n");
		printf("---------------------------------------\n\n");
		keyList();
		printf("\n\n---------------------------------------\n");
	}
	//학년별 명단
	else if (method == 2) {
		printf("\n>>다음은 학년별 명단입니다.\n");
		printf("---------------------------------------\n\n");
		gradeList();
		printf("\n---------------------------------------\n\n");
		
	}
	//전체 명단
	else if (method == 3) {
		printf("\n>>다음은 전체 명단입니다.\n");
		printf("---------------------------------------\n\n");
		printList();
		printf("\n---------------------------------------\n\n");
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//학생관리프로그램 - 학생 추가 시 중복 확인
//-------------------------------------------------------------------------------------------------------------------------------------------------
int CheckStudentNumber(int stdnum)
{
	struct node* ptr = head;
	while (ptr != NULL) {
		//같은 학번을 찾으면 1반환 
		if (stdnum == ptr->key) {
			printf("\n등록되어있는 학번입니다.\n=>(%d %d %s %s %s)\n\n", ptr->key, ptr->grade, ptr->name, ptr->charge, ptr->event);
			return 1;
		}
		ptr = ptr->next;
	}
	//같은 학번을 못찾으면 0반환 
	return 0;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
											//행사관리프로그램 - 학번순에 사용되는 정렬
//-------------------------------------------------------------------------------------------------------------------------------------------------
void SortbyKey(int key, int grade, char* name, char* charge, char* event) {
	
	struct node* newStudent = (struct node*)malloc(sizeof(struct node));
	//새 학생의 데이터추가
	newStudent->key = key;
	newStudent->grade = grade;
	strcpy_s(newStudent->name, 100, name);//첫주소를 넘겨준다
	strcpy_s(newStudent->charge, 2, charge);
	strcpy_s(newStudent->event, 2, event);
	newStudent->next = NULL;

	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드(순차적으로 노드교체)
	struct node* prev_ptr = NULL;//내가 처리하고 있던 전 노드
	ptr = key_head;
	int flag = 0;//맨마지막에 들어갈지 아닐지를 결정해준다

	if (key_head == NULL) {
		key_head = newStudent;

		return;
	}
	while (ptr != NULL) {
		if (ptr->key > newStudent->key) {
			if (prev_ptr == NULL) {
				newStudent->next = ptr;
				key_head = newStudent;
			}
			else {
				newStudent->next = ptr;
				prev_ptr->next = newStudent;
			}
			flag = 1;
			break;
		}
		prev_ptr = ptr;
		ptr = ptr->next;
	}
	if (flag == 0) prev_ptr->next = newStudent;

	
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
											//행사관리프로그램 - 학년순에 사용되는 정렬
//-------------------------------------------------------------------------------------------------------------------------------------------------
void Sortbygrade(int key, int grade, char* name, char* charge, char* event) {
	struct node* newStudent = (struct node*)malloc(sizeof(struct node));
	//새 학생의 데이터추가
	newStudent->key = key;
	newStudent->grade = grade;
	strcpy_s(newStudent->name, 100, name);//첫주소를 넘겨준다
	strcpy_s(newStudent->charge, 2, charge);
	strcpy_s(newStudent->event, 2, event);
	newStudent->next = NULL;

	struct node* ptr;	// 내가 처리하고(가리키고) 있는 노드(순차적으로 노드교체)
	struct node* prev_ptr = NULL;//내가 처리하고 있던 전 노드
	ptr = grade_head;
	int flag = 0;//맨마지막에 들어갈지 아닐지를 결정해준다

	if (grade_head == NULL) {
		grade_head = newStudent;
		
		return;
	}
	while (ptr != NULL) {
		if (ptr->grade > newStudent->grade) {
			if (prev_ptr == NULL) {
				newStudent->next = ptr;
				grade_head = newStudent;
			}
			else {
				newStudent->next = ptr;
				prev_ptr->next = newStudent;
			}
			flag = 1;
			break;
		}
		prev_ptr = ptr;
		ptr = ptr->next;
	}
	if (flag == 0) prev_ptr->next = newStudent;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//학생회비관리프로그램 - 회비내역불러오기 및 노드생성
//-------------------------------------------------------------------------------------------------------------------------------------------------
void moneyAdd(int date, int income, int outcome,int total, char* text) {

	struct money* newMoney = (struct money*)malloc(sizeof(struct money));
	//새 회계내역 데이터추가
	newMoney->date = date;
	newMoney->income = income;
	newMoney->outcome = outcome;
	newMoney->total = total;
	strcpy_s(newMoney->text,100, text);//첫주소를 넘겨준다
	newMoney->next = NULL;
	struct money* ptr;	// 내가 처리하고(가리키고) 있는 노드(순차적으로 노드교체)
	struct money* prev_ptr = NULL;//내가 처리하고 있던 전 노드
	ptr = headMoney;
	int flag = 0;//맨마지막에 들어갈지 아닐지를 결정해준다

	if (headMoney == NULL) {
		headMoney = newMoney;
		return;
	}
	while (ptr != NULL) {
		if (ptr->date > newMoney->date) {
			if (prev_ptr == NULL) {
				newMoney->next = ptr;
				headMoney = newMoney;
			}
			else {
				newMoney->next = ptr;
				prev_ptr->next = newMoney;
			}
			flag = 1;
			break;
		}
		prev_ptr = ptr;
		ptr = ptr->next;
	}
	if (flag == 0) prev_ptr->next = newMoney;

}
//-------------------------------------------------------------------------------------------------------------------------------------------------
		//학생회비관리프로그램 - 총액 계산 및 회비내역작성 //처음에 다 노드로 읽어줬으니 날짜 순서대로 처음부터 써준다.
		//이전 날짜가 이미 있으면 다시입력 요청
//-------------------------------------------------------------------------------------------------------------------------------------------------
void moneyWrite(FILE* fp, FILE* fp2,int date, int income, int outcome, int total, char* text) {

	struct money* ptr = (struct money*)malloc(sizeof(struct money));
	ptr = NULL;
	ptr = headMoney;

	int prev_total = 0;//현재 총계를 계산하기 위해 필요

	fopen_s(&fp2,"학생회비내역최종.txt", "w");//최종 기록으로 전체 데이터를 정렬하여 추가
	fprintf(fp2, "%s %20s %10s %10s %10s", "date", "income", "outcome", "total", "text");
	fprintf(fp2, "\n-------------------------------------------------------------------");
	//마지막 노드 찾기-가장최근에쓴기록이기때문이다.
	while (ptr!=NULL) {
		fprintf(fp2, "\n");
		fprintf(fp2, "%d %12d %10d %10d %10s", ptr->date, ptr->income, ptr->outcome, ptr->total, ptr->text);
		prev_total = ptr->total;
		ptr = ptr->next;
	}
	total = prev_total -outcome + income;

	moneyAdd(date, income, outcome, total, text);//오늘의 행사 기록을 노드마지막에 추가해준다.

	fprintf(fp2, "\n");
	fprintf(fp2, "%d %12d %10d %10d %10s", date, income, outcome, total, text);

	fclose(fp2);

	fopen_s(&fp,"학생회비내역.txt", "a");//읽기용 기록으로 새로운 한 줄만 추가
	fprintf(fp, "\n");
	fprintf(fp, "%d %d %d %d %s", date, income, outcome, total, text);
	fclose(fp);
	
}
