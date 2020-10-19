#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

//�ڵ� ���������� ����Ǿ��� �� "taskkill /im �׽�Ʈ���Դϴ�.exe /f" ��ɾ �����ϰ� �������ָ� �����

//������Ÿ��
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
int smallLetter(char* result);//�������� o,x Ȯ��

struct node {
	int key; //�й�
	int grade;//�г�
	char name[100];//�ִ� 10���� �̸�
	char charge[2];//�л�ȸ�񿩺� o,x
	char event[2];//��� ���� ���� o,x
	struct node* next; //�й� �� ����
};
struct node *head;//ù ��尡 ����.
struct node* grade_head;//�г�� ���
struct node* key_head;//�й��� ���

struct money{
	int date;//��¥
	int income; //�Ա�
	int outcome;//���
	int total;//�ܾ�
	char text[100];//����
	struct money* next;
};
struct money* headMoney;

void main() {
	char file_name[100];//������ �����̸����� ���� ����
	int key = 0; //�й�
	int grade = 0;//�г�
	char name[100];//�ִ� 10���� �̸�
	char charge[2];//�л�ȸ�񿩺� o,x
	char event[2] = { "x" };//��� ���� ���� o,x,����Ʈ��
	int num = 0;//������ ��
	FILE* fp = NULL;
	FILE* fp2 = NULL;
	//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//Dipensive code-�л�����Ʈ
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	fopen_s(&fp,"studentList.txt", "r");
	if (fp == NULL) {
		printf("������ ã�� ���� �����ϴ�.\n");
		return;
	}
	//��� �л� struct�� ����(linked list�� �й���)
	while (fscanf_s(fp, "%d %d %s %s %s", &(key), &(grade), name,100, charge,2, event,2)==5) {
		addStudent(key, grade, name, charge, event);
	}
	fclose(fp);
	
	//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//Dipensive code-ȸ�񳻿�����Ʈ
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	//ȸ�񳻿� �ۼ� �� ����
	int date;
	int income = 0;
	int outcome = 0;
	int total = 0;
	char text[100];

	fopen_s(&fp,"�л�ȸ�񳻿�.txt", "r");
	if (fp == NULL) {
		printf("������ ã�� ���� �����ϴ�.\n");
		return;
	}
	//��� ȸ�񳻿� struct�� ����(��¥��)
	while (fscanf_s(fp, "%d %d %d %d %s", &date, &income,&outcome, &total, text,100) == 5) {
		moneyAdd(date, income, outcome, total, text);
	}
	fclose(fp);

	//-------------------------------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------------------------------
	int program = 100;//���α׷� ���� ����ġ
	int process = 100;//���α׷� ���� ��� ���� ����ġ

	while (1)
	{
		printf("�ȳ��ϼ��� oo�а� �����ڴ� ���α׷��� �����ϰڽ��ϴ�.\n\n");
		printf("\n\n*****���� : �������� ��� �ÿ��� ������ �������ּ���*****\n");
		printf("� ���α׷��� ���ϼ���? (0. ���� 1.�л����� 2.������) : ");
		scanf_s("%d", &program);
		process = 100;//�ʱ�ȭ
		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//�л����� ���α׷�
		//-------------------------------------------------------------------------------------------------------------------------------------------------

		if (program == 1) {
			printf("---------------------------------------\n\n");
			printf("�л����� ���α׷��� �����ϰڽ��ϴ�.\n\n");

			while (process != 0)
			{
				printf("� �۾��� �����Ͻðڽ��ϱ�?\n(0.����/1.�л��߰�/2.�л�����/3.�л���������/4.��ü�л�����) : ");

				scanf_s("%d", &process);
				printf("\n");

				//�л��߰�
				if (process == 1) {

					printf("�߰��� �л��� �й��� �Է����ּ���. :");
					scanf_s("%d", &key);

					//�̹��ִ� �й����� üũ
					if (CheckStudentNumber(key) == 1) {
						continue;
					}


					printf("�߰��� �л��� �г��� �Է����ּ���. :");
					scanf_s("%d", &grade);

					printf("�߰��� �л��� �̸��� �Է����ּ���. :");
					scanf_s("%s", name, 100);

					printf("�߰��� �л��� �л�ȸ�� ���θ� �Է����ּ���.(�ҹ��� o,x) :");
					scanf_s("%s", charge, 2);	

					while (1) {
						if (smallLetter(charge) == 1) {
							printf("������ �л�ȸ�� ���θ� �����ּ���.(�ҹ��� o,x) : ");
							scanf_s("%s", charge, 2);
						}
						else {
							break;
						}

					}

					fopen_s(&fp,"studentList.txt", "a");
					fprintf(fp, "%d %d %s %s %s\n", key, grade,name, charge, event);//���Ͽ��߰�(���Ͽ��� �л����� �������x)
					fclose(fp);

					addStudent(key, grade, name, charge, event);//��� ���� ó��
				}

				//�л�����
				else if (process == 2) {

					while (1) {
						printf("������ �л��� �й��� �Է����ּ���. :");
						scanf_s("%d", &key);
						if (CheckStudentNumber(key) == 0) {
							printf("�ش� �л��� �������� �ʽ��ϴ�.\n\n");
						}
						else {
							break;
						}
					}

					deleteStudent(key);
					//�л���� ������ �л� ���� �ٽ� �Է�
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

				//�л����� �����ϱ�
				else if (process == 3) {
					//������ �ִ��� Ȯ���ϱ�
					while (1) {
						printf("������ ������ �����Ͻðڽ��ϱ�?(�й��� �Է����ּ���) :");
						scanf_s("%d", &key);
						if (CheckStudentNumber(key) == 0) {
							printf("�ش� �л��� �������� �ʽ��ϴ�.\n\n");
						}
						else {
							break;
						}
					}

					use_modify(key);

					//������ ������ �ٽ� �ۼ�
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

				//��ü �л� ���
				else if (process == 4) {

					printList();
				}
				else if (process == 0) {
					printf("\n\n�л����� ���α׷��� �����ϰڽ��ϴ�.\n");
					printf("\n\n---------------------------------------\n\n");

					Sleep(3000);//3�� �ȳ����� �����ֱ����� ��ġ
					system("cls");//���ο� ������ ���� ȭ�� Ŭ����
				}
				else {
					printf("�߸� �� �Է� �� �Դϴ�. �ٽ� �Է����ּ���.\n\n");
				}
			}
		}
		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//������ ���α׷�
		//-------------------------------------------------------------------------------------------------------------------------------------------------

		else if (program == 2) {
			int method = 0;//�л���� ���� ��� ����ġ
			int selected = 100;//�ٸ���� or �������� ����
			printf("---------------------------------------\n\n");
			printf("������ ���α׷��� �����ϰڽ��ϴ�.\n");
			printf("��� ���ϸ��� �Է����ּ���.(ex.�������.txt): ");
			scanf_s("%s", file_name, 100);
			printf("\n\n");

			fopen_s(&fp,file_name, "a");//�б�� ���� ������ ������ ���������� , ������ ���

			printf("�л������ � ������� ���ðڽ��ϱ�?\n(1.�й���/2.�г⺰/3.��ü����): ");
			scanf_s("%d", &method);
			diverseList(method);

			while (selected != 0) {
				printf("\n� �۾��� �����Ͻðڽ��ϱ�? (1.�ٸ� ����� ���/2.�������� ����/0.����):");
				scanf_s("%d", &selected);
				if (selected == 1) {
					printf("\n� ������� ����� ���ðڽ��ϱ�?\n(1.�й���/2.�г⺰/3.��ü����) : ");
					scanf_s("%d", &method);
					printf("\n\n");
					diverseList(method);
				}
				else if (selected == 2) {
					while (1) {
						printf("\n�й��� �Է����ֽø� �������ΰ� o�� ����˴ϴ�.(0�� �����ֽø� ������ ����˴ϴ�.)\n: ");
						scanf_s("%d", &key);
						if (key == 0) {
							break;
						}
						event_modify(key, fp, &num);
						//������������� ������ �������� �ʴ´� �ֳ��ϸ� ������ ����� ���� �����⵵�ϰ� ���� ��綧 ����
					}
				}
				else if (selected == 0) {
					printf("\n\n������ ���α׷��� �����ϰڽ��ϴ�.\n");
					printf("---------------------------------------\n\n");
					fprintf(fp, "----------------------------------------------------------------------------------\n");
					fprintf(fp, "\n�� %d���� �л��� �����ϼ̽��ϴ�.", num);
					printf("������ ��� �����ۼ��� �Ϸ�Ǿ����ϴ�.\n\n");
					fclose(fp);

					int last_date;
					printf("��� ��¥�� �����Դϱ�? (ex. 20190301) : ");
					scanf_s("%d", &date);

					
					struct money* ptr = (struct money*)malloc(sizeof(struct money));
					ptr=headMoney;
					//������ ��¥
					while (ptr != NULL) {
						last_date = ptr->date;
						ptr = ptr->next;
					}
					free(ptr);

					//������ ��¥���� �� ��¥�� �ٽ� �Է��� �޴´�. (total��꿡 ȥ���� ���� �ʱ� ����)
					while (1) {
						if (date < last_date) {
							printf("��� ��¥�� ������ ��¥���� �� ��¥ �Դϴ�.\n\n");
							printf("��� ��¥�� �����Դϱ�? (ex. 20190301) : ");
							scanf_s("%d", &date);
						}
						else {
							break;
						}
					}

					printf("��� �Աݾ��� ���Դϱ�? : ");
					scanf_s("%d", &income);


					printf("��� ��ݾ��� ���Դϱ�? : ");
					scanf_s("%d", &outcome);

					printf("������ �ۼ����ּ���. : ");
					scanf_s("%s", text, 100);

					moneyWrite(fp,fp2, date, income, outcome, total, text);

					printf("�л�ȸ�� ������ �����߽��ϴ�.\n\n");

					Sleep(3000);//3�� �ȳ����� �����ֱ����� ��ġ
					system("cls");//���ο� ������ ���� ȭ�� Ŭ����
				}
				else {
					printf("\n���� �۾��Դϴ�. �ٽ��Է����ּ���.\n\n");
				}
			}
		}

		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//���α׷� ����
		//-------------------------------------------------------------------------------------------------------------------------------------------------
		else if (program == 0) {
			printf("---------------------------------------\n\n");
			printf("���α׷��� �����ϰڽ��ϴ�.\n");
			break;
		}


		//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//�߸� �� �� �Է�
		//-------------------------------------------------------------------------------------------------------------------------------------------------
		else {
			printf("�߸� �� �Է� �� �Դϴ�. �ٽ� �Է����ּ���.\n\n");
		}

	}


	
}





//-------------------------------------------------------------------------------------------------------------------------------------------------
																	//����� ���� �Լ�
//-------------------------------------------------------------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------------------------------------------------------------
											//�ʱ��л�����߰�,�л��������α׷� - �л� �߰��Լ�
//-------------------------------------------------------------------------------------------------------------------------------------------------
void addStudent(int key, int grade, char* name, char* charge, char* event)
{

	struct node* newStudent = (struct node*)malloc(sizeof(struct node));
	//�� �л��� �������߰�
	newStudent->key = key;
	newStudent->grade = grade;
	strcpy_s(newStudent->name,100, name);//ù�ּҸ� �Ѱ��ش�
	strcpy_s(newStudent->charge,2, charge);
	strcpy_s(newStudent->event,2, event);
	newStudent->next = NULL;
	
	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���(���������� ��屳ü)
	struct node* prev_ptr = NULL;//���� ó���ϰ� �ִ� �� ���
	ptr = head;
	int flag = 0;//�Ǹ������� ���� �ƴ����� �������ش�

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
											//�л��������α׷� - �л������Լ� (key�� �й�)
//-------------------------------------------------------------------------------------------------------------------------------------------------
void deleteStudent(int key)
{
	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���
	struct node* prev_ptr = NULL; //���� ó���ߴ� ������ ���
	ptr = head;

	while (ptr != NULL) {
		if (ptr->key == key) {
			//��� ù ���� ���� ��
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
											//�л��������α׷� - �л���ü����Լ�
//-------------------------------------------------------------------------------------------------------------------------------------------------
void printList()
{
	int num=0;
	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���
	ptr = head;
	printf("%8s%15s\t%20s\t%13s\t%10s\n","�й�","�̸�","�г�","�л�ȸ�񿩺�","�����������");
	printf("----------------------------------------------------------------------------------\n");
	
	while (ptr != NULL) {
		
		printf("%10d%15s\t%10d\t%10s\t%10s\n", ptr->key, ptr->name, ptr->grade, ptr->charge, ptr->event);
		num++;
		ptr = ptr->next;
	}
	printf("----------------------------------------------------------------------------------\n");
	printf("\n�� %d���� �л��� �˻��Ǿ����ϴ�.\n\n", num);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//�л��������α׷� - �������������
//-------------------------------------------------------------------------------------------------------------------------------------------------
void use_modify(int key) {
	int modify = 100;//� ������ �����Ұ��� ����ġ
	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���

	ptr = head;

	
	//��忡�� ���� �й��� Ž��
	
	while (ptr != NULL) {
		if (ptr->key == key) {
			printf("\n�л��� � ������ �����Ͻðڽ��ϱ�?\n");
			printf("(1.�й�/2.�г�/3.�̸�/4.�л�ȸ�񿩺�/0.��������): ");
			scanf_s("%d", &modify);
			if (modify == 1) {
				printf("������ �й��� �����ּ���. : ");
				scanf_s("%d", &(ptr->key));
				printf("�й������� �Ϸ�Ǿ����ϴ�.");
			}
			else if (modify == 2) {
				printf("������ �г��� �����ּ���. : ");
				scanf_s("%d", &(ptr->grade));
				printf("�г������ �Ϸ�Ǿ����ϴ�.\n");

			}
			else if (modify == 3) {
				printf("������ �̸��� �����ּ���. : ");
				scanf_s("%s", ptr->name, 100);
				printf("�̸������� �Ϸ�Ǿ����ϴ�.\n");

			}
			else if (modify == 4) {
				printf("������ �л�ȸ�񿩺θ� �����ּ���.(�ҹ��� o,x) : ");
				scanf_s(" %s", ptr->charge, 2);
				//�ҹ���Ȯ��
				while (1) {
					if (smallLetter(ptr->charge) == 1) {
						printf("������ �л�ȸ�񿩺θ� �����ּ���.(�ҹ��� o,x) : ");
						scanf_s("%s", ptr->charge, 2);
					}
					else {
						break;
					}
				}
			}	
			else if (modify == 0) {
				printf("�л� ������ ��ġ�ڽ��ϴ�.\n\n");
				return;
			}
		}
		else{
			ptr = ptr->next;
		}
	}
	printf("ã���ô� �л��� ������ �����ϴ�.\n\n");
		
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
											//�л��������α׷�-�ҹ���o,xȮ��
//-------------------------------------------------------------------------------------------------------------------------------------------------
int smallLetter(char* result) {
	if (*result == 'o'|| *result == 'x') {
		printf("\n�Ϸ�Ǿ����ϴ�.\n\n");
		return 0;
	}
	else {
		printf("o �Ǵ� x�� �ƴմϴ� �ٽ� �Է����ּ���.\n\n");
		return 1;
	}
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
											//���������α׷� - ����������θ� �����ϱ�
//-------------------------------------------------------------------------------------------------------------------------------------------------
void event_modify(int key, FILE* fp, int* num) {
	int modify = 100;//� ������ �����Ұ��� ����ġ
	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���
	ptr = head;
	//��忡�� ���� �й��� Ž��->�������μ���
	while (ptr != NULL) {
		if (ptr->key == key) {
			strcpy_s(ptr->event,2, "o");
			printf("�������ΰ� ����Ǿ����ϴ�.\n\n");
			fprintf(fp, "%d %s %d %s %s\n", ptr->key, ptr->name, ptr->grade, ptr->charge, ptr->event);
			(*num)++;
		}
		ptr = ptr->next;
	}
	if(ptr == NULL && *num==0) {
	printf("ã���ô� �л��� ������ �����ϴ�.\n");
	}

	
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
											//���������α׷� - �г⺰ ������
//-------------------------------------------------------------------------------------------------------------------------------------------------

void gradeList() {
	FILE* fp = NULL;
	int key = 0; //�й�
	int grade = 0;//�г�
	char name[100];//�ִ� 10���� �̸�
	char charge[2];//�л�ȸ�񿩺� o,x
	char event[2] = { "x" };//��� ���� ���� o,x,����Ʈ��
	

	fopen_s(&fp, "studentList.txt", "r");
	if (fp == NULL) {
		printf("������ ã�� ���� �����ϴ�.\n");
		return;
	}
	while (fscanf_s(fp, "%d %d %s %s %s", &(key), &(grade), name, 100, charge, 2, event, 2) == 5) {
		Sortbygrade(key, grade, name, charge, event);
	}

	//���
	int count_grade = 1;//1�г���ͽ���
	int num = 0;
	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���
	ptr = grade_head;

	while (ptr != NULL) {
		printf(">>%d�г�\n", count_grade);
		printf("%8s%15s\t%20s\t%13s\t%10s\n", "�г�", "�̸�", "�й�", "�л�ȸ�񿩺�", "�����������");
		printf("----------------------------------------------------------------------------------\n");
		while(ptr != NULL && ptr->grade == count_grade){
			printf("%10d%15s\t%10d\t%10s\t%10s\n", ptr->grade, ptr->name, ptr->key, ptr->charge, ptr->event);
			num++;
			ptr = ptr->next;
		}
		printf("----------------------------------------------------------------------------------\n");
		printf("\n�� %d���� �л��� �˻��Ǿ����ϴ�.\n\n", num);
		num = 0;
		count_grade++;
	}
	fclose(fp);
}



//-------------------------------------------------------------------------------------------------------------------------------------------------
											//���������α׷� - �й��� ������(2014~2019)
//-------------------------------------------------------------------------------------------------------------------------------------------------
void keyList() {
	FILE* fp = NULL;
	int key = 0; //�й�
	int grade = 0;//�г�
	char name[100];//�ִ� 10���� �̸�
	char charge[2];//�л�ȸ�񿩺� o,x
	char event[2] = { "x" };//��� ���� ���� o,x,����Ʈ��


	fopen_s(&fp, "studentList.txt", "r");
	if (fp == NULL) {
		printf("������ ã�� ���� �����ϴ�.\n");
		return;
	}
	while (fscanf_s(fp, "%d %d %s %s %s", &(key), &(grade), name, 100, charge, 2, event, 2) == 5) {
		SortbyKey(key, grade, name, charge, event);
	}

	//���
	int count_key = 201400000;//1�г���ͽ���
	int num = 0;
	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���
	ptr = key_head;

	while (ptr != NULL) {
		printf(">>%d�й�\n", count_key / 100000);
		printf("%8s%15s\t%20s\t%13s\t%10s\n", "�й�", "�̸�", "�г�", "�л�ȸ�񿩺�", "�����������");
		printf("----------------------------------------------------------------------------------\n");

		while (ptr != NULL&&ptr->key/100000==count_key/100000) {
			printf("%10d%15s\t%10d\t%10s\t%10s\n", ptr->key, ptr->name, ptr->grade, ptr->charge, ptr->event);
			num++;
			ptr = ptr->next;
		}
		printf("----------------------------------------------------------------------------------\n");
		printf("\n�� %d���� �л��� �˻��Ǿ����ϴ�.\n\n", num);
		num = 0;
		count_key+=100000;
	}
	fclose(fp);
	
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//���������α׷� - �پ��� ����� ���
//-------------------------------------------------------------------------------------------------------------------------------------------------
void diverseList(int method) {
	int j = 1;//�г� ī��Ʈ
	struct node* ptr;   // ���� ó���ϰ�(����Ű��) �ִ� ���
	ptr = head;

	//�г⺰ ���
	if (method == 1) {
		printf("\n>>������ �й��� ����Դϴ�.\n");
		printf("---------------------------------------\n\n");
		keyList();
		printf("\n\n---------------------------------------\n");
	}
	//�г⺰ ���
	else if (method == 2) {
		printf("\n>>������ �г⺰ ����Դϴ�.\n");
		printf("---------------------------------------\n\n");
		gradeList();
		printf("\n---------------------------------------\n\n");
		
	}
	//��ü ���
	else if (method == 3) {
		printf("\n>>������ ��ü ����Դϴ�.\n");
		printf("---------------------------------------\n\n");
		printList();
		printf("\n---------------------------------------\n\n");
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
											//�л��������α׷� - �л� �߰� �� �ߺ� Ȯ��
//-------------------------------------------------------------------------------------------------------------------------------------------------
int CheckStudentNumber(int stdnum)
{
	struct node* ptr = head;
	while (ptr != NULL) {
		//���� �й��� ã���� 1��ȯ 
		if (stdnum == ptr->key) {
			printf("\n��ϵǾ��ִ� �й��Դϴ�.\n=>(%d %d %s %s %s)\n\n", ptr->key, ptr->grade, ptr->name, ptr->charge, ptr->event);
			return 1;
		}
		ptr = ptr->next;
	}
	//���� �й��� ��ã���� 0��ȯ 
	return 0;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
											//���������α׷� - �й����� ���Ǵ� ����
//-------------------------------------------------------------------------------------------------------------------------------------------------
void SortbyKey(int key, int grade, char* name, char* charge, char* event) {
	
	struct node* newStudent = (struct node*)malloc(sizeof(struct node));
	//�� �л��� �������߰�
	newStudent->key = key;
	newStudent->grade = grade;
	strcpy_s(newStudent->name, 100, name);//ù�ּҸ� �Ѱ��ش�
	strcpy_s(newStudent->charge, 2, charge);
	strcpy_s(newStudent->event, 2, event);
	newStudent->next = NULL;

	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���(���������� ��屳ü)
	struct node* prev_ptr = NULL;//���� ó���ϰ� �ִ� �� ���
	ptr = key_head;
	int flag = 0;//�Ǹ������� ���� �ƴ����� �������ش�

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
											//���������α׷� - �г���� ���Ǵ� ����
//-------------------------------------------------------------------------------------------------------------------------------------------------
void Sortbygrade(int key, int grade, char* name, char* charge, char* event) {
	struct node* newStudent = (struct node*)malloc(sizeof(struct node));
	//�� �л��� �������߰�
	newStudent->key = key;
	newStudent->grade = grade;
	strcpy_s(newStudent->name, 100, name);//ù�ּҸ� �Ѱ��ش�
	strcpy_s(newStudent->charge, 2, charge);
	strcpy_s(newStudent->event, 2, event);
	newStudent->next = NULL;

	struct node* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���(���������� ��屳ü)
	struct node* prev_ptr = NULL;//���� ó���ϰ� �ִ� �� ���
	ptr = grade_head;
	int flag = 0;//�Ǹ������� ���� �ƴ����� �������ش�

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
											//�л�ȸ��������α׷� - ȸ�񳻿��ҷ����� �� ������
//-------------------------------------------------------------------------------------------------------------------------------------------------
void moneyAdd(int date, int income, int outcome,int total, char* text) {

	struct money* newMoney = (struct money*)malloc(sizeof(struct money));
	//�� ȸ�賻�� �������߰�
	newMoney->date = date;
	newMoney->income = income;
	newMoney->outcome = outcome;
	newMoney->total = total;
	strcpy_s(newMoney->text,100, text);//ù�ּҸ� �Ѱ��ش�
	newMoney->next = NULL;
	struct money* ptr;	// ���� ó���ϰ�(����Ű��) �ִ� ���(���������� ��屳ü)
	struct money* prev_ptr = NULL;//���� ó���ϰ� �ִ� �� ���
	ptr = headMoney;
	int flag = 0;//�Ǹ������� ���� �ƴ����� �������ش�

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
		//�л�ȸ��������α׷� - �Ѿ� ��� �� ȸ�񳻿��ۼ� //ó���� �� ���� �о������� ��¥ ������� ó������ ���ش�.
		//���� ��¥�� �̹� ������ �ٽ��Է� ��û
//-------------------------------------------------------------------------------------------------------------------------------------------------
void moneyWrite(FILE* fp, FILE* fp2,int date, int income, int outcome, int total, char* text) {

	struct money* ptr = (struct money*)malloc(sizeof(struct money));
	ptr = NULL;
	ptr = headMoney;

	int prev_total = 0;//���� �Ѱ踦 ����ϱ� ���� �ʿ�

	fopen_s(&fp2,"�л�ȸ�񳻿�����.txt", "w");//���� ������� ��ü �����͸� �����Ͽ� �߰�
	fprintf(fp2, "%s %20s %10s %10s %10s", "date", "income", "outcome", "total", "text");
	fprintf(fp2, "\n-------------------------------------------------------------------");
	//������ ��� ã��-�����ֱٿ�������̱⶧���̴�.
	while (ptr!=NULL) {
		fprintf(fp2, "\n");
		fprintf(fp2, "%d %12d %10d %10d %10s", ptr->date, ptr->income, ptr->outcome, ptr->total, ptr->text);
		prev_total = ptr->total;
		ptr = ptr->next;
	}
	total = prev_total -outcome + income;

	moneyAdd(date, income, outcome, total, text);//������ ��� ����� ��帶������ �߰����ش�.

	fprintf(fp2, "\n");
	fprintf(fp2, "%d %12d %10d %10d %10s", date, income, outcome, total, text);

	fclose(fp2);

	fopen_s(&fp,"�л�ȸ�񳻿�.txt", "a");//�б�� ������� ���ο� �� �ٸ� �߰�
	fprintf(fp, "\n");
	fprintf(fp, "%d %d %d %d %s", date, income, outcome, total, text);
	fclose(fp);
	
}
