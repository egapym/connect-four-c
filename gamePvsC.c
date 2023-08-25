#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<stdint.h>

//visual studioでは動かない
//#include<unistd.h>
//#include<sys/time.h>

#define disp 1
#define disp2 0
#define disp3 0
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable:4996 )

/*処理時間の測定*/
//double gettimeofday_sec()
//{
//	struct timeval tv;
//	gettimeofday(&tv, NULL);
//	return tv.tv_sec + tv.tv_usec * 1e-6; /*秒に変換*/
//}

//評価値
double p1 = 8; //---o
double p2 = 8; //--o-
double p3 = 32; //--oo
double p4 = 8; //-o--
double p5 = 32; //-o-o
double p6 = 32; //-oo-
double p7 = 64; //-ooo
double p8 = 8; //o---
double p9 = 32; //o--o
double p10 = 32; //o-o-
double p11 = 64; //o-oo
double p12 = 32; //oo--
double p13 = 64; //oo-o
double p14 = 64; //ooo-
double p15 = 0; //oooo
double p16 = 32; //
double p17 = 32; //
double p18 = 32; //
double p19 = 32; //


				  //double p1 = 0; //---o
				  //double p2 = 0; //--o-
				  //double p3 = 0; //--oo
				  //double p4 = 0; //-o--
				  //double p5 = 0; //-o-o
				  //double p6 = 0; //-oo-
				  //double p7 = 0; //-ooo
				  //double p8 = 0; //o---
				  //double p9 = 0; //o--o
				  //double p10 = 0; //o-o-
				  //double p11 = 0; //o-oo
				  //double p12 = 0; //oo--
				  //double p13 = 0; //oo-o
				  //double p14 = 0; //ooo-
				  //double p15 = 0; //oooo
				  //double p16 = 0; //o-
				  //				  //oo
				  //double p17 = 0; //-o
				  //				  //oo
				  //double p18 = 0; //oo
				  //				  //oo
				  //double p19 = 10000;

int ban[12][12] = { 0 }; //盤面を格納
int ban_1[12][12] = { 0 }; //trapreach関数で使用。先手のtraprech場所に1が格納される
int ban_2[12][12] = { 0 }; //trapreach関数で使用。後手のtraprech場所に2が格納される
int kifu_count = 0,/*棋譜を格納*/ ie = 0,/*eval関数で使用*/ je = 10;/*eval関数で使用*/
uint64_t ct = 0, ct2 = 12;/*評価回数*/
int cpu = -1; //CPUが置く手の列名を格納
int turn = 1; //対局が始まる順番を格納
int not[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }; //自分が置いた次の手で相手が勝利する場所→そこには置かない
int not2[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int air_not[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int yes[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int yes2[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int y_n_init[12] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int wre[4] = { 0 }; //置いたらWリーチになる場所が2箇所ある場合、[0],[1]に値が格納される
double node_max[12] = { 0 }; //深さx12の配列が必要。とりあえず最大10層まで。深さごとの節の評価最大値を格納
int cp3 = 0; //cpuの順番　絶対に値は変わらない
double ab1 = 1, ab2 = 1;
int ret = 0, dep_count = 0;
char kifu[144] = { 0 };
char input[144] = { 0 };
int player_i = 0;
int player_j = 0;
int cpu_bef2_i = 0;
int cpu_bef2_j = 0;
int eval_sort[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ,11 };
int trapreach_flag = 0;
int check_air_wreach_1_flag = 0;
int check_air_wreach_2_flag = 0;
int cp_reach[12] = { 0 };
int cp_flag1 = 0;
int cp_flag2 = 0;

void display(void); //現在の盤面を表示
int retry(int player); //局面を戻す
int plays(int player); //プレイヤーの手
int plays_v(int player); //データを読み込んで対局させるための関数
int plays_cpu(int turn_cpu); //CPUの手
int check(int check_i, int check_j, int player); //盤に箱が4つ並んでいるか調べる
int check_v(int check_i, int check_j, int cp); //置いた場所から盤の箱が4つ並んでいるか調べる
int check_v2(int player); //与えられた局面に対して「箱が4つ並ぶ」次の手を求める(yesの場所以外)
int check_v3(int check_i, int check_j, int cp); //置いた場所から盤の箱が4つ並んでいるか調べる monimax用
int check_LR(int check_i, int check_j, int player);
int check_UD(int check_i, int check_j, int player);
int check_LURD(int check_i, int check_j, int player);
int check_RULD(int check_i, int check_j, int player);

int top(int cp, int j, int *check_i, int *check_j); //盤の高さを調べる
int sw(int cp); //先手、後手を一時的に入れ替える(1↔2)
int check_eval(int cp); //yes,noを探す(天井まで積まれていた場合notにする)
int check_eval2(int check_i, int check_j, int cp); //箱が4つ並ぶか調べる
int check_evalx(int cp, int fi); //yes,noを探す(指定した列のみ+天井まで積まれていたらnotにする)
int check_wreach(int cp); //Wリーチを探す
int vic_cp(int cp); //確定勝利の手を求める
double eval1(int check_i, int check_j); //評価関数（先手）
double eval2(int check_i, int check_j); //評価関数（後手）
int may_vic(int cp);
int minimax(int cp); //ミニマックス法（2手先）
int alphabeta3(int cp); //αβ法(3手先)
int alphabeta4(int cp); //αβ法(4手先)
int alphabeta(int cp); //αβ法(5手先)
int trapreach(int cp); //トラップリーチ
int check_air_wreach(int cp); //空中Wリーチを調べる

main(int argc, char *argv[]) {
	FILE *fp1;
	int player, i = 0, j = 0, x = 0, y, result = 0, result_first, result_second, flag = 0;
	int c, te, dep1, dep2;
	char input_x[8], input_y[8];

	srand((unsigned int)time(NULL));

	if (argc < 2) {
		printf("文字を入力してください。\n");
		return;
	}
	c = *argv[1];
	if (c == 'O' || c == 'o') {
		player = 1; //X
	}
	else if (c == 'X' || c == 'x') {
		player = 2; //O
	}
	else if (c == 'C' || c == 'c') {
		player = 0; //CPU戦
	}
	else {
		printf("O、またはXを入力してください。\n");
		return;
	}

	/*ファイルオープン*/
	char *fname = "kifu.txt";
	fp1 = fopen(fname, "r");

	/* ファイルオープン */
	if ((fp1 = fopen(fname, "r")) == NULL) {
		fprintf(stderr, "%s\n", "ファイルを読み込めませんでした。");
		return;
	}
	while (fscanf(fp1, "%s", &kifu[x]) != EOF) {
		input[x] = kifu[x];
		x++;
	}
	te = 999; //棋譜の上限

			  //配列に0を格納
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 12; j++) {
			ban[i][j] = 0;
		}
	}
	//if (player != 0) {
	//	printf("探索する深さを決めてください: ");

	//	fgets(input_y, sizeof(input_y), stdin);
	//	x = -1;
	//	if (input_y[0] == 'r' || input_y[0] == 'R') {
	//		x = 99;
	//	}
	//	if (x == -1) {
	//		ini_depth = atoi(input_y);
	//	}
	//	if (x == 99) {
	//		flag = 1;
	//	}
	//	//    printf("ini_depth = %d\n",ini_depth);
	//	//    scanf("%d",&ini_depth);
	//}
	//else {
	//	printf("先手の探索する深さを決めてください: ");
	//	scanf("%d", &dep1);
	//	printf("後手の探索する深さを決めてください: ");
	//	scanf("%d", &dep2);
	//}

	//対局情報の読み込み(先攻は1、後攻は2)
	while (1) {
		result_first = plays_v(1);
		if (result_first == 1) {
			printf("先攻の勝利です。\n");
			return;
		}
		else if (result_first == -1) {
			break;
		}
		else if (kifu_count == te) {
			printf("%d手になったので引き分けとします。\n", te); break;
		}
		else if (kifu_count == 144) {
			printf("置ける場所が無くなったので引き分けとします。\n"); break;
		}
		if (cp3 == 1) {
			cpu_bef2_i = player_i;
			cpu_bef2_j = player_j;
		}
		result_second = plays_v(2);
		if (result_second == 1) {
			printf("後攻の勝利です。\n");
			break;
		}
		else if (result_second == -1) {
			break;
		}
		else if (kifu_count == te) {
			printf("%d手になったので引き分けとします。\n", te); break;
		}
		else if (kifu_count == 144) {
			printf("置ける場所が無くなったので引き分けとします。\n"); break;
		}
		if (cp3 == 2) {
			cpu_bef2_i = player_i;
			cpu_bef2_j = player_j;
		}
	}

	display();

	//  printf("turn = %d, player = %d\n\n",turn,player);

	if (player == 2) {//CPUが先手の時
		ab2 = -1.1;
	}
	else {
		ab1 = -1.1;
	}

	if (player == 1 && turn == 1 && kifu_count == 0) {
		cp3 = 2;
		while (1) {
		play1b:
		play2a:
			result = plays(1);
			if (result == 1) {
				printf("先手の勝利です。\nプレイヤーの手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re1:
					y = retry(1);
					if (y % 2 == 0) {
						goto play1a;
					}
					else {
						goto play1b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re1;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		play1a:
		play2b:
			result = plays_cpu(2);
			if (result == 1) {
				printf("後手の勝利です。\nプレイヤーの手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(2);
					if (y % 2 == 0) {
						goto play2a;
					}
					else {
						goto play2b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		}
	}
	//対局(先攻がプレイヤーの場合)
	if (player == 1 && turn == 1 && result == 0) {
		cp3 = 2;
		while (1) {
		play11b:
		play12a:
			result = plays_cpu(2);
			if (result == 1) {
				printf("後手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(2);
					if (y % 2 == 0) {
						goto play11a;
					}
					else {
						goto play11b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		play11a:
		play12b:
			result = plays(1);
			if (result == 1) {
				printf("先手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re6:
					y = retry(1);
					if (y % 2 == 0) {
						goto play12a;
					}
					else {
						goto play12b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re6;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		}
	}
	else if (player == 2 && turn == 1 && kifu_count == 0) {
		cp3 = 1;
		while (1) {
		play3b:
		play4a:
			result = plays_cpu(1);
			if (result == 1) {
				printf("先手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(1);
					if (y % 2 == 0) {
						goto play3a;
					}
					else {
						goto play3b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		play3a:
		play4b:
			result = plays(2);
			if (result == 1) {
				printf("後手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re2:
					y = retry(2);
					if (y % 2 == 0) {
						goto play4a;
					}
					else {
						goto play4b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re2;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		}
	}
	else if (player == 2 && turn == 1 && result == 0) {
		cp3 = 1;
		while (1) {
		play5b:
		play6a:
			result = plays(2);
			if (result == 1) {
				printf("後手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re3:
					y = retry(2);
					if (y % 2 == 0) {
						goto play5a;
					}
					else {
						goto play5b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re3;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		play6b:
		play5a:
			result = plays_cpu(1);
			if (result == 1) {
				printf("先手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(1);
					if (y % 2 == 0) {
						goto play6a;
					}
					else {
						goto play6b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		}
	}
	else if (player == 1 && turn == 2) {
		//対局(後攻がプレイヤーの場合)
		cp3 = 2;
		while (1) {
		play7b:
		play8a:
			result = plays(1);
			if (result == 1) {
				printf("先手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re4:
					y = retry(1);
					if (y % 2 == 0) {
						goto play7a;
					}
					else {
						goto play7b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re4;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		play7a:
		play8b:
			result = plays_cpu(2);
			if (result == 1) {
				printf("後手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(2);
					if (y % 2 == 0) {
						goto play8a;
					}
					else {
						goto play8b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		}
	}
	else if (player == 2 && turn == 2) {
		//対局(後攻がプレイヤーの場合)
		cp3 = 1;
		while (1) {
		play9b:
		play10a:
			result = plays_cpu(1);
			if (result == 1) {
				printf("先手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(1);
					if (y % 2 == 0) {
						goto play9a;
					}
					else {
						goto play9b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		play9a:
		play10b:
			result = plays(2);
			if (result == 1) {
				printf("後手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re5:
					y = retry(2);
					if (y % 2 == 0) {
						goto play10a;
					}
					else {
						goto play10b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re5;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
		}
	}
	else if ((player == 0 && turn == 2) || (player == 0 && turn == 1 && kifu_count == 0)) {
		//対局(後攻がプレイヤーの場合)
		while (1) {
		play13b:
		play14a:
			cp3 = 1;
			if (cp3 == 1) {//CPUが先手の時
				ab2 = -ab2;
				ab1 = abs(ab1);
			}
			else {
				ab1 = -ab1;
				ab2 = abs(ab2);
			}
			//			ini_depth = dep1;
			result = plays_cpu(1);
			if (result == 1) {
				printf("先手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(1);
					if (y % 2 == 0) {
						goto play13a;
					}
					else {
						goto play13b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
			fgets(input_x, sizeof(input_x), stdin);
			if (input_x[0] == 'E' || input_x[0] == 'e') {
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
			}
		play13a:
		play14b:
			cp3 = 2;
			if (cp3 == 1) {//CPUが先手の時
				ab2 = -ab2;
				ab1 = abs(ab1);
			}
			else {
				ab1 = -ab1;
				ab2 = abs(ab2);
			}
			//			ini_depth = dep2;
			result = plays_cpu(2);
			if (result == 1) {
				printf("後手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re7:
					y = retry(2);
					if (y % 2 == 0) {
						goto play14a;
					}
					else {
						goto play14b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re7;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
			fgets(input_x, sizeof(input_x), stdin);
			if (input_x[0] == 'E' || input_x[0] == 'e') {
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
			}
		}
	}
	else if (player == 0 && turn == 1) {
		//対局(後攻がプレイヤーの場合)
		while (1) {
		play15b:
		play16a:
			cp3 = 1;
			if (cp3 == 1) {//CPUが先手の時
				ab2 = -ab2;
				ab1 = abs(ab1);
			}
			else {
				ab1 = -ab1;
				ab2 = abs(ab2);
			}
			//			ini_depth = dep1;
			result = plays_cpu(2);
			if (result == 1) {
				printf("後手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
					y = retry(1);
					if (y % 2 == 0) {
						goto play15a;
					}
					else {
						goto play15b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
			fgets(input_x, sizeof(input_x), stdin);
			if (input_x[0] == 'E' || input_x[0] == 'e') {
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
			}
		play15a:
		play16b:
			cp3 = 2;
			if (cp3 == 1) {//CPUが先手の時
				ab2 = -ab2;
				ab1 = abs(ab1);
			}
			else {
				ab1 = -ab1;
				ab2 = abs(ab2);
			}
			//			ini_depth = dep2;
			result = plays_cpu(1);
			if (result == 1) {
				printf("先手の勝利です。\n手を戻す場合は「R」を、対局を終了する場合は何かを入力してください: ");
				fgets(input_x, sizeof(input_x), stdin);
				if (input_x[0] == 'R' || input_x[0] == 'r') {
				re8:
					y = retry(2);
					if (y % 2 == 0) {
						goto play16a;
					}
					else {
						goto play16b;
					}
				}
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
				break;
			}
			else if (result == 2) {
				ret = 1;
				goto re8;
			}
			else if (kifu_count == te) {
				printf("%d手になったので引き分けとします。\n", te); break;
			}
			else if (kifu_count == 144) {
				printf("置ける場所が無くなったので引き分けとします。\n"); break;
			}
			fgets(input_x, sizeof(input_x), stdin);
			if (input_x[0] == 'E' || input_x[0] == 'e') {
				printf("\n棋譜\n");
				for (i = 0; i < kifu_count; i++) {
					printf("%c\n", kifu[i]);
				}
				printf("\n");
			}
		}
	}

	fclose(fp1);
	return 0;
}

//盤を表示する
void display(void) {
	int i, j;

	//盤を表示
	for (i = 0; i < 12; i++) {
		for (j = 0; j < 12; j++) {
			if (ban[i][j] == 0) {
				printf("・");
			}
			else if (ban[i][j] == 1) {
				printf("Ｏ");
			}
			else if (ban[i][j] == 2) {
				printf("Ｘ");
			}

		}
		printf("\n");
	}

	//列の名前を表示
	for (i = 0; i < 12; i++) {
		printf("―");
	}
	printf("\n");
	printf("０１２３４５６７８９ＡＢ");
	printf("\n\n");

	return;
}

int retry(int player) {
	int i, j, x = 0, y, z;
	char re[8];
	/*
	if(player != cp3){
	x = 1;
	}*/
	if (ret == 1) {
		x = 0;
	}

	while (1) {
		printf("戻す手数を入力してください: ");
		fgets(re, sizeof(re), stdin);
		y = -1;
		if ('0' <= re[0] && re[0] <= '9') {
			y = re[0] - '0';
		}
		if (0 <= x && x <= kifu_count - 1) {
			break;
		}
		else {
			printf("正しい値を入力してください\n");
		}
	}
	for (j = 1; j < y + 1 - x; j++) {
		//	      printf("kifu[%d] = %c\n",kifu_count-j,kifu[kifu_count-j]);
		if ('0' <= kifu[kifu_count - j] && kifu[kifu_count - j] <= '9') {
			z = kifu[kifu_count - j] - '0';
		}
		else if (kifu[kifu_count - j] == 'A' || kifu[kifu_count - j] == 'a') {
			z = 10;
		}
		else if (kifu[kifu_count - j] == 'B' || kifu[kifu_count - j] == 'b') {
			z = 11;
		}
		for (i = 0; i < 12; i++) {
			//	printf("ban[%d][%d] = %d\n",i,z,ban[i][z]);
			if (ban[i][z] != 0) {
				ban[i][z] = 0;
				break;
			}
		}
	}
	kifu_count = kifu_count - (y - x);
	printf("\n");
	display();
	if (ret == 1) {
		y++;
	}
	ret = 0;
	return y;
}


//先攻、後攻の手
int plays(int player) {
	int x, i, flag = 1, result;
	char input_x[8];
	while (1) {
		if (player == 1) {
			printf("先手の順番です。Ｏ? ");
		}
		else {
			printf("後手の順番です。Ｘ? ");
		}
		fgets(input_x, sizeof(input_x), stdin);
		kifu[kifu_count] = input_x[0];
		x = -1;
		if ('0' <= input_x[0] && input_x[0] <= '9') {
			x = input_x[0] - '0';
		}
		else if (input_x[0] == 'A' || input_x[0] == 'a') {
			x = 10;
		}
		else if (input_x[0] == 'B' || input_x[0] == 'b') {
			x = 11;
		}
		//Eを入力した場合、これまでの棋譜を表示する
		else if (input_x[0] == 'E' || input_x[0] == 'e') {
			flag = 0;
			printf("\n棋譜\n");
			for (i = 0; i < kifu_count; i++) {
				printf("%c\n", kifu[i]);
			}
			printf("\n");
		}
		else if (input_x[0] == 'R' || input_x[0] == 'r') {
			return 2;
		}
		if ((ban[0][x] == 1 || ban[0][x] == 2) && x != -1) {
			printf("その場所には置けません。別の場所に置いてください。\n");
			flag = 0;
		}
		if (0 <= x && x <= 11 && flag == 1) {
			break;
		}
		else if (flag == 1) {
			printf("0～9かA,Bを入力してください。\n");
		}
		flag = 1;
	}
	//入力された列名から配列に1を代入し、盤を表示する
	for (i = 11; i >= 0; i--) {
		if (ban[i][x] == 0) {
			ban[i][x] = player;
			result = check(i, x, player);
			display();
			player_i = i;
			player_j = x;
			break;
		}
	}
	kifu_count++;
	return result;
}

//CPUの手
int plays_cpu(int turn_cpu) {
	int i, j, p, flag = 0, flag2 = 0, flag3 = 0, result = 0, x = 0, flg = 0, ch = 0, fg7 = 0, temp_count = 0;
	int check_i_plays_cpu1, check_j_plays_cpu1;
	double sum, t1, t2, t3, t4, eval;
	char str[10] = { 0 };
	int temp, temp2, te_last;

	unsigned int sleep(unsigned int seconds);

	if (turn_cpu == 1) {
		printf("先手の順番です。Ｏ? ");
	}
	else {
		printf("後手の順番です。Ｘ? ");
	}
	printf("\n");
	//  sleep(1);

	//1,与えられた局面に対して「箱が4つ並ぶ」次の手を求める
	flag = check_v2(turn_cpu);

	//CPUの初手
	if (kifu_count == 0) {
		cpu = 5;
		flag = 1;
	}

	////CPUの2手目
	if (kifu_count == 1) {
		for (i = 0; i < 12; i++) {
			if (ban[11][i] == 1) {
				if (2 < i && i < 9) {
					cpu = i;
					break;
				}
				else if (i < 3) {
					cpu = 3;
					break;
				}
				else if (8 < i) {
					cpu = 8;
					break;
				}
			}
		}
		flag = 1;
	}

	//2,与えられた局面に対して「箱が4つ並ぶことを防ぐ」次の手を求める
	if (flag == 0) {
		flag = check_v2(sw(turn_cpu));
		if (flag == 1 && disp == 1) {
			printf("４つ防ぐ\n");
		}
	}
	//3,自分が置いた次の手で相手が勝利する場所を求める→そこには置かない
	check_eval(turn_cpu);

	//4,相手が置いた次の手で自分が勝利する場所を求める→そこにはなるべく置かないほうが良いため
	check_eval(sw(turn_cpu));

	memcpy(not2, not, sizeof(not));
	memcpy(yes2, yes, sizeof(yes));

	for (i = 0; i < 12; i++) { //最大まで積まれていないか調べる
		if (ban[0][i] != 0)
			not2[i] = 88;
	}
	//ban[9][3] = 2;
	//	flag = check_air_wreach(9,3,sw(turn_cpu));

	//5,置いたらリーチになる場所が2つ以上ある場所を求める
	if (flag == 0) {
		flag = check_wreach(turn_cpu);
		//   printf("わわわ flag = %d\n",flag);
		if (flag == 1 && disp == 1) {
			printf("Wリーチ発見\n");
		}
	}
	check_eval(turn_cpu);
	check_eval(sw(turn_cpu));

	for (i = 0; i < (sizeof(wre) / sizeof(wre[0])); i++) {
		wre[i] = -1;
	}

	//6,相手が、置いたらリーチになる場所が2つ以上ある場所を求める
		// Wリーチを防ぐ場所が2箇所以上ある場合、防げる場所以外のnotを1にしてどっちに置くべきか処理させる
		if (flag == 0) {
			for (i = 0; i < 12; i++) {
			turn_cpu_1:
				while (1) {
					if (not2[i] >= 0 || yes2[i] >= 0) {
						i++;
						if (i > 11) {
							break;
						}
					}
					else {
						break;
					}
				}
				if (i > 11) {
					break;
				}
				p = top(turn_cpu, i, &check_i_plays_cpu1, &check_j_plays_cpu1);
				if (p == 1) {
					i++;
					if (i > 11) {
						break;
					}
					goto turn_cpu_1;
				}

				if (check_wreach(sw(turn_cpu)) == 1) {
					not2[i] = check_i_plays_cpu1;
				}
				else {
					temp_count++;
				}
				ban[check_i_plays_cpu1][check_j_plays_cpu1] = 0;
			}
			if (temp_count == 1) {
				flag = 0;
				if (flag == 1 && disp == 1) {
					printf("Wリーチ防ぐ\n");
				}
			}
		}

	//自分の箱をリーチするように置いていくと勝利が確定する手を求める
	if (flag == 0) {
		dep_count = 0;
		flag = vic_cp(turn_cpu);
		if (flag == 1 && disp == 1) {
			printf("%d手先くらいで勝利確定！\n", dep_count);
		}
	}

	//相手の箱をリーチするように置いていくと敗北が確定する手を求める
	if (flag == 0) {
		display();
		for (i = 0; i < 12; i++) {
			while (1) {
				while (1) {
					if (not2[i] >= 0 || yes2[i] >= 0) {
						i++;
						if (i > 11) {
							break;
						}
					}
					else {
						break;
					}
				}
				if (i > 11) {
					break;
				}
				p = top(turn_cpu, i, &check_i_plays_cpu1, &check_j_plays_cpu1);
				if (p == 1) {
					i++;
					if (i > 11) {
						break;
					}
				}
				if (check_v(check_i_plays_cpu1, check_j_plays_cpu1, sw(turn_cpu)) == 0) {
					break;
				}
				else {
					ban[check_i_plays_cpu1][check_j_plays_cpu1] = 0;
					i++;
				}
			}
			if (i > 11) {
				flag = 0;
				break;
			}
			dep_count = 0;

			flag = vic_cp(sw(turn_cpu));

			//         printf("flag = %d, i = %d, cpu = %d\n",flag,i,cpu);
			if (dep_count > 55) {
				if (disp == 1) {
					//	printf("55手以上先読みした？\n");
				}
			}
			ban[check_i_plays_cpu1][check_j_plays_cpu1] = 0;
			if (flag == 1 && dep_count < 66) {
				if (disp == 1) {
					printf("%dに置いたら敗北の可能性あり cpu = %d\n", i, cpu);
				}
				fg7 = 1;
				not2[i] = check_i_plays_cpu1;
				ch++;
			}
			flag = 0;
		}
		check_eval(turn_cpu);
		check_eval(sw(turn_cpu));

		if (ch > 0 && ch < 12 && fg7 == 1) {
			if (disp == 1) {
				printf("敗北を未然に防いだ\n");
			}
		}
		if (ch == 12) {
			for (i = 0; i < 12; i++) {
				not2[i] = not[i];
			}
			for (i = 0; i < 12; i++) { //最大まで積まれていないか調べる
				if (ban[0][i] != 0) {
					not2[i] = 88;
				}
			}
			if (disp == 1) {
				printf("敗北確定！aa\n");
			}
			//www2(turn_cpu);
		}
	}


	if (flag == 0) {
		flag = may_vic(turn_cpu);
		if (flag == 1 && disp == 1) {
			printf("may_vic\n");
		}
	}

	if (disp == 1) {
		for (i = 0; i < 12; i++) {
			printf("not2[%d] = %d\n", i, not2[i]);
		}

		for (i = 0; i < 12; i++) {
			printf("yes2[%d] = %d\n", i, yes2[i]);
		}
	}

	//先手の場合は相手が地面に箱を積むまで上に置く
	/*
	if (syote_flag == 0 && cp3 == 1) {
	if (ban[0][5] != 0) {
	syote_flag = 1;
	}
	if (syote_flag == 0) {
	for (i = 0; i<12; i++) {
	if (ban[11][i] == sw(cp3)) {
	syote_flag = 1;
	break;
	}
	}
	if (syote_flag == 0) {
	cpu = 5;
	flag = 1;
	}
	}
	}
	*/

	if (flag == 0) {
		ch = 0;
		for (i = 0; i < 12; i++) {
			if (not2[i] >= 0 || yes2[i] >= 0) {
				ch++;
			}
		}

		if (ch == 12) {
			check_eval(sw(turn_cpu));
			check_eval(turn_cpu);
			for (i = 0; i < 12; i++) {
				if (not[i] == -1 && yes[i] >= 0) {
					cpu = i;
					flag = 1;
					if (disp == 1) {
						printf("本格的に置く場所無いよ\n");
					}
					break;
				}
			}
			if (flag == 0) {
				flg = 1;
			}
		}
		if (ch == 11) {
			for (i = 0; i < 12; i++) {
				if (not2[i] == -1 && yes2[i] == -1) {
					cpu = i;
					flag = 1;
					if (disp == 1) {
						printf("置ける場所が一箇所しかない\n");
					}
					break;
				}
			}
		}
		if (flg == 1 && ch == 12) {
			for (i = 0; i < 12; i++) {
				if (ban[0][i] == 0) {
					cpu = i;
					flag = 1;
					if (disp == 1) {
						printf("詰み\n");
					}
					break;
				}
			}
		}
	}

	//CPUが打てる残り手数を求める
	te_last = 144;
	te_last = te_last - kifu_count;
	for (i = 0; i < 12; ++i) {
		//天井まで積まれている
		//if (not2[i] == 88) {
		//	te_last = te_last - 12;
		//	//天井ではないが置けない場所
		//}
	    if (not2[i] == 99) { //vic_cpで置けなくなった場所
			top(turn_cpu, i, &check_i_plays_cpu1, &check_j_plays_cpu1);
			te_last = te_last - (12 - check_i_plays_cpu1);
			ban[check_i_plays_cpu1][check_j_plays_cpu1] = 0;
		}////////////////
		else if (not2[i] >= 0 && yes2[i] >= 0 && not2[i] != 88) {
			top(turn_cpu, i, &check_i_plays_cpu1, &check_j_plays_cpu1);
			if (not2[i] < yes2[i]) {
				te_last = te_last - (not2[i] + 2);
			}
			else {
				te_last = te_last - (yes2[i] + 2);
			}
			ban[check_i_plays_cpu1][check_j_plays_cpu1] = 0;
		}
		else if (not2[i] >= 0 && yes2[i] == -1 && not2[i] != 88) {
			top(turn_cpu, i, &check_i_plays_cpu1, &check_j_plays_cpu1);
			te_last = te_last - (not2[i] + 2);
			ban[check_i_plays_cpu1][check_j_plays_cpu1] = 0;
		}
		else if (yes2[i] >= 0 && not2[i] == -1) {
			top(turn_cpu, i, &check_i_plays_cpu1, &check_j_plays_cpu1);
			te_last = te_last - (yes2[i] + 2);
			ban[check_i_plays_cpu1][check_j_plays_cpu1] = 0;
		}
	}



	//9,α-β法
	if (flag == 0) {
		je = 3;
		for (j = je; j >= 3; j--) {
			if (ban[j][0] == 0 && ban[j][1] == 0 && ban[j][2] == 0 && ban[j][3] == 0 && ban[j][4] == 0 && ban[j][5] == 0 && ban[j][6] == 0 && ban[j][7] == 0 && ban[j][8] == 0 && ban[j][9] == 0 && ban[j][10] == 0 && ban[j][11] == 0) {
				je = j;
				break;
			}
		}
		minimax(turn_cpu);

		/* 評価値を昇順にソート */
		for (i = 0; i<12; ++i) {
			for (j = i; j<12; ++j) {
				if (node_max[i] < node_max[j]) {
					temp = node_max[i];
					temp2 = eval_sort[i];

					node_max[i] = node_max[j];
					eval_sort[i] = eval_sort[j];

					node_max[j] = temp;
					eval_sort[j] = temp2;
				}
			}
		}

		printf("\n");

		//残り手数が5以下になった時の処理
		if (te_last <= 4) {
			if (te_last == 4) {
				//		t1 = gettimeofday_sec(); /*処理時間計測開始*/
				alphabeta4(turn_cpu);
				//		t2 = gettimeofday_sec(); /*処理時間計測終了*/
			}
			else if (te_last == 3) {
				//		t1 = gettimeofday_sec(); /*処理時間計測開始*/
				alphabeta3(turn_cpu);
				//		t2 = gettimeofday_sec(); /*処理時間計測終了*/
			}
			else if (te_last == 2) {
				//		t1 = gettimeofday_sec(); /*処理時間計測開始*/
				minimax(turn_cpu);
				//		t2 = gettimeofday_sec(); /*処理時間計測終了*/
			}
		}
		else {
			//	t1 = gettimeofday_sec(); /*処理時間計測開始*/
			alphabeta(turn_cpu);
			//	t2 = gettimeofday_sec(); /*処理時間計測終了*/
		}

		// for (i = 0; i < 12 * ini_depth - 12; i++) {
		// 	if (i == 0) {
		// 		printf("1手先\n");
		// 	}
		// 	else if (i % 12 == 0) {
		// 		printf("\n%d手先\n", (i / 12) + 1);
		// 	}
		// 	printf("[%d] = %.0f\n", i % 12, node_max[i]);

		// }

		ct2 = pow(12, 5);

		sum = (double)ct / (double)ct2;
		sum = (1 - sum) * 100;

		printf("実行した評価回数 = %llu, 本来の評価回数 = %llu, 評価省略回数 = %llu, %.2f％の短縮\n", ct, ct2, ct2 - ct, sum);
		//	printf("処理時間:%.8f秒\n", t2 - t1);

		eval = node_max[0];
		cpu = 0;
		for (i = 1; i < 12; i++) {
			if (eval < node_max[i] && node_max[i] != -INFINITY) {
				eval = node_max[i];
				cpu = i;
			}
			else if (eval == node_max[i] && node_max[i] != -INFINITY) {
				if (i < 5) {
					if (abs(5 - cpu) > abs(5 - i)) {
						cpu = i;
					}
				}
				else {
					if (abs(6 - cpu) > abs(6 - i)) {
						cpu = i;
					}
				}
			}
		}

	}

	x = cpu;

	printf("CPUが選択した手: ");
	if (x < 10) {
		printf("%d\n", x);
		sprintf(str, "%d", x);
	}
	else if (x == 10) {
		printf("A\n");
		sprintf(str, "a");
	}
	else if (x == 11) {
		printf("B\n");
		sprintf(str, "b");
	}
	kifu[kifu_count] = str[0];
	for (i = 11; i >= 0; i--) {
		if (ban[i][x] == 0) {
			ban[i][x] = turn_cpu;
			result = check(i, x, turn_cpu);
			display();
			cpu_bef2_i = i;
			cpu_bef2_j = x;
			break;
		}
	}
	trapreach(turn_cpu);
	//初期化
	for (i = 0; i < 12; i++) {
		eval_sort[i] = i;
	}

	kifu_count++;
	cpu = -1;
	return result;
}

//先攻、後攻の手
int plays_v(int player) {
	int x, i, flag = 1, result;
	char input_x[8];

	while (1) {
		input_x[0] = input[kifu_count];
		x = -1;
		if ('0' <= input_x[0] && input_x[0] <= '9') {
			x = input_x[0] - '0';
		}
		else if (input_x[0] == 'A' || input_x[0] == 'a') {
			x = 10;
		}
		else if (input_x[0] == 'B' || input_x[0] == 'b') {
			x = 11;
		}
		if (ban[0][x] == 1 || ban[0][x] == 2) {
			return -1;
		}
		if (0 <= x && x <= 11 && flag == 1) {
			break;
		}
		return -1;
	}

	//入力された列名から配列に値を代入する
	for (i = 11; i >= 0; i--) {
		if (ban[i][x] == 0) {
			ban[i][x] = player;
			result = check(i, x, player);
			break;
		}
	}
	kifu_count++;
	turn = player;
	return result;
}

//盤の箱が4つ並んでいるか調べる
int check(int check_i, int check_j, int player) {
	int flag = 0;

	//***駒を置いた左右3マスを調べる***
	flag = check_LR(check_i, check_j, player);
	if (flag == 1) {
		return flag;
	}

	//***駒を置いた場所から上下3マスを調べる***
	flag = check_UD(check_i, check_j, player);
	if (flag == 1) {
		return flag;
	}

	//***駒を置いた場所から左上～右下3マスを調べる***
	flag = check_LURD(check_i, check_j, player);
	if (flag == 1) {
		return flag;
	}

	//***駒を置いた場所から右上～左下3マスを調べる***
	flag = check_RULD(check_i, check_j, player);
	if (flag == 1) {
		return flag;
	}

	return flag;
}

//置いた場所から盤の箱が4つ並んでいるか調べる
int check_v(int check_i, int check_j, int cp) {
	int i, x_s = -3, x_e = 3, p = 0, flag = 0;
	int check_i2, check_j2;

	if (check_j - 3 < 0) {
		x_s = 0 - check_j;
	}
	if (check_j + 3 > 11) {
		x_e = 11 - check_j;
	}

	for (i = x_s; i <= x_e; i++) {
	check_v2:

		p = top(cp, i + check_j, &check_i2, &check_j2);
		if (p == 1) {
			i++;
			if (i > x_e) {
				return 0;
			}
			goto check_v2;
		}

		//***駒を置いた左右3マスを調べる***
		flag = check_LR(check_i2, check_j2, cp);

		//***駒を置いた場所から上下3マスを調べる***
		if (flag == 0) {
			flag = check_UD(check_i2, check_j2, cp);
		}

		//***駒を置いた場所から左上～右下3マスを調べる***
		if (flag == 0) {
			flag = check_LURD(check_i2, check_j2, cp);
		}

		//***駒を置いた場所から右上～左下3マスを調べる***
		if (flag == 0) {
			flag = check_RULD(check_i2, check_j2, cp);
		}

		ban[check_i2][check_j2] = 0;

		if (flag == 1) {
			cpu = check_j2;
			return 1;
		}
	}

	return 0;
}

//箱が4つ並ぶか調べる
int check_v2(int player) {
	int i, a = 0, p = 0, flag = 0;
	int check_i, check_j;

	//  display();

	while (1) {

		while (1) {
			for (i = 11; i >= 0; i--) {
				//     printf("ban[%d][%d] = %d \n",i,p,ban[i][p]);
				if (ban[i][p] == 0) {
					check_i = i;
					check_j = p;
					ban[check_i][check_j] = player;
					a = 1;
					break;
				}
			}
			if (a == 1) {
				break;
			}
			p++;
			if (p > 11) {
				return 0;
			}
		}
		a = 0;

		//***駒を置いた左右3マスを調べる***
		flag = check_LR(check_i, check_j, player);
		if (flag == 1) {
			cpu = check_j;
		}

		//***駒を置いた場所から上下3マスを調べる***
		if (flag == 0) {
			flag = check_UD(check_i, check_j, player);
			if (flag == 1) {
				cpu = check_j;
			}
		}

		//***駒を置いた場所から左上～右下3マスを調べる***
		if (flag == 0) {
			flag = check_LURD(check_i, check_j, player);
			if (flag == 1) {
				cpu = check_j;
			}
		}

		//***駒を置いた場所から右上～左下3マスを調べる***
		if (flag == 0) {
			flag = check_RULD(check_i, check_j, player);
			if (flag == 1) {
				cpu = check_j;
			}
		}

		ban[check_i][check_j] = 0;
		p++;
		if (p > 11 || flag == 1) {
			break;
		}
	} //while文の終わり

	return flag;
}

//置いた場所から盤の箱が4つ並んでいるか調べる monimax用
int check_v3(int check_i, int check_j, int cp) {
	int i, x_s = -3, x_e = 3, p = 0, flag = 0;
	int check_i2, check_j2;

	if (check_j - 3 < 0) {
		x_s = 0 - check_j;
	}
	if (check_j + 3 > 11) {
		x_e = 11 - check_j;
	}

	for (i = x_s; i <= x_e; i++) {
	check_v3:

		p = top(cp, i + check_j, &check_i2, &check_j2);
		if (p == 1) {
			i++;
			if (i > x_e) {
				return -1;
			}
			goto check_v3;
		}

		//***駒を置いた左右3マスを調べる***
		flag = check_LR(check_i2, check_j2, cp);

		//***駒を置いた場所から上下3マスを調べる***
		if (flag == 0) {
			flag = check_UD(check_i2, check_j2, cp);
		}

		//***駒を置いた場所から左上～右下3マスを調べる***
		if (flag == 0) {
			flag = check_LURD(check_i2, check_j2, cp);
		}

		//***駒を置いた場所から右上～左下3マスを調べる***
		if (flag == 0) {
			flag = check_RULD(check_i2, check_j2, cp);
		}

		ban[check_i2][check_j2] = 0;

		if (flag == 1) {
			return i + check_j;
		}
	}

	return -1;
}

int check_LR(int check_i, int check_j, int player) {
	int i, x_s, x_e, y = -3, check = 0, flag = 0;

	if (check_j - 4 < 0) {
		x_s = 0 - check_j;
	}
	else {
		x_s = -4;
	}
	if (check_j + 4 > 11) {
		x_e = 11 - check_j;
	}
	else {
		x_e = 4;
	}

	//***駒を置いた左右3マスを調べる***
	for (i = x_s; i <= x_e; i++) {
		if (ban[check_i][check_j + i] == player) { //4回連続で配列の値に1を発見したらリターン
			check++;
			if (check == 4) {
				flag = 1;
				return flag;
			}
		}
		else if (ban[check_i][check_j + i] != player) { //配列に0を発見したらcheck = 0
			check = 0;
		}
	}
	return flag;
}

int check_UD(int check_i, int check_j, int player) {
	int i, y_s, y_e, x = -3, check = 0, flag = 0;

	if (check_i - 4 < 0) {
		y_s = 0 - check_i;
	}
	else {
		y_s = -4;
	}
	if (check_i + 4 > 11) {
		y_e = 11 - check_i;
	}
	else {
		y_e = 4;
	}

	//***駒を置いた場所から上下3マスを調べる***
	for (i = y_s; i <= y_e; i++) {
		if (ban[check_i + i][check_j] == player) {
			check++;
			if (check == 4) {
				flag = 1;
				return flag;
			}
		}
		else if (ban[check_i + i][check_j] != player) {
			check = 0;
		}
	}
	return flag;
}

int check_LURD(int check_i, int check_j, int player) {
	int i, x = -3, y = -3, check = 0, flag = 0;

	//***駒を置いた場所から左上～右下3マスを調べる***
	while (1) { //調べる配列がマイナスにならないようにする
		if (check_i + x < 0 || check_j + y < 0) {
			x++;
			y++;
		}
		else {
			break;
		}
	}
	for (i = 0; i < 7; i++) {
		//調べる配列が11を越える、または調べる範囲を越えた場合
		if (check_i + x > 11 || check_i + x > check_i + 3) {
			break;
		}
		if (check_j + y > 11 || check_j + y > check_j + 3) {
			break;
		}
		//    printf("x = %d, y = %d, check_i = %d, ban[%d][%d] = %d \n",x,y,check_i,check_i+x,check_j+y,ban[check_i + x][check_j+y]);
		if (ban[check_i + x][check_j + y] == player) {
			check++;
			if (check == 4) {
				flag = 1;
				return flag;
			}
		}
		else if (ban[check_i + x][check_j + y] != player) {
			check = 0;
		}
		x++;
		y++;
	}
	return flag;
}

int check_RULD(int check_i, int check_j, int player) {
	int i, x = -3, y = 3, check = 0, flag = 0;

	//***駒を置いた場所から右上～左下3マスを調べる***
	while (1) { //調べる配列がマイナスにならないようにする
		if (check_i + x < 0 || check_j + y > 11) {
			x++;
			y--;
		}
		else {
			break;
		}
	}

	for (i = 0; i < 7; i++) {
		//調べる配列が11を越える、または調べる範囲を越えた場合
		if (check_i + x > 11 || check_i + x > check_i + 3) {
			//          printf("oooooooooooooooo check_i = %d, x = %d\n",check_i,x);
			break;
		}
		if (check_j + y < 0 || check_j + y < check_j - 3) {
			//      printf("xxxxxxxxxxxxxxxxxx\n");
			break;
		}

		if (ban[check_i + x][check_j + y] == player) {
			check++;
			if (check == 4) {
				flag = 1;
				return flag;
			}
		}
		else if (ban[check_i + x][check_j + y] != player) {
			check = 0;
		}
		x++;
		y--;
	}
	return flag;
}

// int cp の1と2を入れ替える
int sw(int cp) {
	if (cp == 1) {
		cp = 2;
	}
	else {
		cp = 1;
	}
	return cp;
}

int top(int cp, int j, int *check_i, int *check_j) {

	int i, a = 0;

	while (1) {
		for (i = 11; i >= 0; i--) {
			//     printf("ban[%d][%d] = %d \n",i,p,ban[i][p]);
			if (ban[i][j] == 0) {
				*check_i = i;
				*check_j = j;
				ban[*check_i][*check_j] = cp;
				a = 1;
				break;
			}
		}
		if (a == 1) {
			break;
		}
		if (a == 0) {
			return 1;
		}
	}

	return 0;
}

int check_eval(int cp) {

	int i, j, p = 0, flag = 0, flag2 = 0;
	int check_i, check_j, evod;

	if (cp == cp3) {
		memcpy(not, y_n_init, sizeof(y_n_init));
	}

	if (cp != cp3) {
		memcpy(yes, y_n_init, sizeof(y_n_init));
	}

	for (i = 0; i < 12; i++) {

	p1:
		for (j = 0; j < 2; j++) {
			while (1) {
				p = top(cp, i, &check_i, &check_j);
				if (p == 1 && j == 0) {
					not[i] = 88;
					i++;
					if (i == 12) {
						return 0;
					}
				}
				else if (p == 1 && j == 1) {
					ban[check_i][check_j] = 0;
					cp = 3 - cp; // cpの1と2を入れ替え
					i++;
					if (i == 12) {
						return 0;
					}
					goto p1;
				}
				else {
					cp = 3 - cp; // cpの1と2を入れ替え
					break;
				}
			}
		}
		//***駒を置いた左右3マスを調べる***
		flag = check_LR(check_i, check_j, sw(cp));
		if (flag == 1) {
			if (cp == cp3) {
				not[check_j] = check_i;
			}
			else {
				yes[check_j] = check_i;
			}
		}

		//***駒を置いた場所から左上～右下3マスを調べる***
		if (flag == 0) {
			flag = check_LURD(check_i, check_j, sw(cp));
			if (flag == 1) {
				if (cp == cp3) {
					not[check_j] = check_i;
				}
				else {
					yes[check_j] = check_i;
				}
			}
		}

		//***駒を置いた場所から右上～左下3マスを調べる***
		if (flag == 0) {
			flag = check_RULD(check_i, check_j, sw(cp));
			if (flag == 1) {
				if (cp == cp3) {
					not[check_j] = check_i;
				}
				else {
					yes[check_j] = check_i;
				}
			}
		}

		//   printf("check_i = %d, check_j = %d\n",check_i,check_j);
		ban[check_i][check_j] = 0;
		if (flag2 == 0) {
			ban[check_i + 1][check_j] = 0;
		}
		flag2 = 0;
	}   //for文iの終わり

	return 0;
}

//おいた場所から
int check_eval2(int check_i, int check_j, int cp) {

	int i, j, x_s = -3, x_e = 3, p = 0, flag = 0, flag2 = 0;
	int check_i2, check_j2, evod;

	if (check_j - 3 < 0) {
		x_s = 0 - check_j;
	}
	if (check_j + 3 > 11) {
		x_e = 11 - check_j;
	}

	if (cp == cp3) {
		for (i = x_s; i <= x_e; i++) {
			not[i + check_j] = -1;
		}
	}

	if (cp != cp3) {
		for (i = x_s; i <= x_e; i++) {
			yes[i + check_j] = -1;
		}
	}

	for (i = x_s; i <= x_e; i++) {

	p1:
		for (j = 0; j < 2; j++) {
			while (1) {
				p = top(cp, i + check_j, &check_i2, &check_j2);
				if (p == 1 && j == 0) {
					not[i + check_j] = 88; //天井
					i++;
					if (i > x_e) {
						return 0;
					}
				}
				else if (p == 1 && j == 1) {
					ban[check_i2][check_j2] = 0;
					cp = 3 - cp; // cpの1と2を入れ替え
					i++;
					if (i > x_e) {
						return 0;
					}
					goto p1;
				}
				else {
					cp = 3 - cp; // cpの1と2を入れ替え
					break;
				}
			}
		}
		//***駒を置いた左右3マスを調べる***
		flag = check_LR(check_i2, check_j2, sw(cp));
		if (flag == 1) {
			if (cp == cp3) {
				not[check_j2] = check_i2;
			}
			else {
				yes[check_j2] = check_i2;
			}
		}

		//***駒を置いた場所から左上～右下3マスを調べる***
		if (flag == 0) {
			flag = check_LURD(check_i2, check_j2, sw(cp));
			if (flag == 1) {
				if (cp == cp3) {
					not[check_j2] = check_i2;
				}
				else {
					yes[check_j2] = check_i2;
				}
			}
		}

		//***駒を置いた場所から右上～左下3マスを調べる***
		if (flag == 0) {
			flag = check_RULD(check_i2, check_j2, sw(cp));
			if (flag == 1) {
				if (cp == cp3) {
					not[check_j2] = check_i2;
				}
				else {
					yes[check_j2] = check_i2;
				}
			}
		}
		//		display();
		//   printf("check_i = %d, check_j = %d\n",check_i,check_j);
		ban[check_i2][check_j2] = 0;
		if (flag2 == 0) {
			ban[check_i2 + 1][check_j2] = 0;
		}
		flag2 = 0;
	}   //for文iの終わり

	return 0;
}

//指定した列のみ判定する
int check_evalx(int cp, int fi) {

	int i, p = 0, flag = 0, flag2 = 0;
	int check_i, check_j, evod;

	if (cp == cp3) {
		not[fi] = -1;
	}

	if (cp != cp3) {
		yes[fi] = -1;
	}

	for (i = 0; i < 2; i++) {
		while (1) {
			p = top(cp, fi, &check_i, &check_j);
			if (p == 1 && i == 0) {
				not[fi] = 88; //天井
				return 0;
			}
			else if (p == 1 && i == 1) {
				ban[check_i][check_j] = 0;
				cp = 3 - cp; // cpの1と2を入れ替え
				return 0;
			}
			else {
				cp = 3 - cp; // cpの1と2を入れ替え
				break;
			}
		}
	}
	//***駒を置いた左右3マスを調べる***
	flag = check_LR(check_i, check_j, sw(cp));
	if (flag == 1) {
		if (cp == cp3) {
			not[check_j] = check_i;
		}
		else {
			yes[check_j] = check_i;
		}
	}

	//***駒を置いた場所から左上～右下3マスを調べる***
	if (flag == 0) {
		flag = check_LURD(check_i, check_j, sw(cp));
		if (flag == 1) {
			if (cp == cp3) {
				not[check_j] = check_i;
			}
			else {
				yes[check_j] = check_i;
			}
		}
	}

	//***駒を置いた場所から右上～左下3マスを調べる***
	if (flag == 0) {
		flag = check_RULD(check_i, check_j, sw(cp));
		if (flag == 1) {

			if (cp == cp3) {
				not[check_j] = check_i;
			}
			else {
				yes[check_j] = check_i;
			}
		}
	}

	//   printf("check_i = %d, check_j = %d\n",check_i,check_j);
	ban[check_i][check_j] = 0;
	if (flag2 == 0) {
		ban[check_i + 1][check_j] = 0;
	}
	//   flag2 = 0;
	//for文iの終わり

	return 0;
}

//ダブルリーチしていないか調べる
int check_wreach(int cp) {

	int i, c = 0, k = 0, p = 0, y = -3, x = 0, flag = 0, flag2 = 0, flag3 = 0, flag4 = 0, flg = 0, flg2 = 0, flg3 = 0, flg4 = 0, check;
	int check_i2, check_j2;
	int check_j3, check_i3, check_i4, check_j4;
	int x_s2 = -3, x_e2 = 3;
	int not_check_wreach[12], yes_check_wreach[12];

	check_eval(sw(cp));
	check_eval(cp);
	memcpy(not_check_wreach, not, sizeof(not));
	memcpy(yes_check_wreach, yes, sizeof(yes));

	for (i = 0; i < 12; i++) {
	g1:

		if (yes[i] >= 0 && cp == cp3) {
			flg = 1;
			goto a;
		}
		else {
			flg = 0;
		}
		if (not[i] >= 0 && cp != cp3) {
			flg = 1;
			//      printf("ああ\n");
			goto a;
		}
		else {
			flg = 0;
		}
	a:
		if (cp == cp3) {
			while (1) {
				if (not2[i] >= 0 || not[i] >= 0) {
					i++;
					if (i > 11) {
						break;
					}
					goto g1;
				}
				else {
					break;
				}
			}
			if (i > 11) {
				break;
			}
		}
		if (cp != cp3) {
			while (1) {
				if (yes2[i] >= 0 || yes[i] >= 0) {
					i++;
					if (i > 11) {
						break;
					}
					goto g1;
				}
				else {
					break;
				}
			}
			if (i > 11) {
				break;
			}
		}

		p = top(cp, i, &check_i2, &check_j2);
			//display();
		if (p == 1) {
			i++;
			if (i == 12) {
				return 0;
			}
			goto g1;
		}

		check_eval(sw(cp));
		check_eval(cp);

		if (flg == 1) {
			if (cp == cp3 && yes_check_wreach[i] >= 0) {
				flag3++;
			}
			else if (cp != cp3 && not_check_wreach[i] >= 0) {
				flag3++;
			}
		}

		if (yes_check_wreach[i] >= 0 && cp == cp3) {
			p = top(sw(cp), i, &check_i3, &check_j3);
			//	display();
			if (p != 1) {
				p = top(cp, i, &check_i4, &check_j4);
				//	display();
				if (p != 1) {
					flg2 = check_LR(check_i4, check_j4, cp);

					if (flg2 == 0) {
						flg2 = check_UD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_LURD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_RULD(check_i4, check_j4, cp);
					}
					ban[check_i4][check_j4] = 0;
				}

				if (flg2 == 1) {
					cpu = i;
					ban[check_i3][check_j3] = 0;
					ban[check_i2][check_j2] = 0;
					//      printf("ああ\n");
					return 1;
				}
				ban[check_i3][check_j3] = 0;
			}
		}
		if (not_check_wreach[i] >= 0 && cp != cp3) {
			//            printf("いい");
			p = top(sw(cp), i, &check_i3, &check_j3);
			if (p != 1) {
				p = top(cp, i, &check_i4, &check_j4);
				if (p != 1) {
					flg2 = check_LR(check_i4, check_j4, cp);

					if (flg2 == 0) {
						flg2 = check_UD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_LURD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_RULD(check_i4, check_j4, cp);
					}
					ban[check_i4][check_j4] = 0;
				}

				if (flg2 == 1) {
					cpu = i;
					ban[check_i3][check_j3] = 0;
					ban[check_i2][check_j2] = 0;
					// 	     printf("いい\n");
					return 1;
				}
				ban[check_i3][check_j3] = 0;
			}
		}

		if (cp == cp3) {
			check_eval(sw(cp3));
		}
		else {
			check_eval(cp3);
		}

		if (i - 3 < 0) {
			x_s2 = 0 - i;
		}
		if (i + 3 > 11) {
			x_e2 = 11 - i;
		}

		//***駒を置いた左右3マスを調べる***
		for (k = x_s2; k <= x_e2; k++) {
		check_wreach2:

			p = top(cp, i + k, &check_i3, &check_j3);
			//		display();
			if (p == 1) {
				k++;
				if (k > x_e2) {
					break;
				}
				goto check_wreach2;
			}

			//***駒を置いた左右3マスを調べる***
			flag = check_LR(check_i2, check_j2, cp);
			if (flag == 1) {
				flag2 = 1;
			}

			//***駒を置いた場所から上下3マスを調べる***
			if (flag == 0) {
				flag = check_UD(check_i2, check_j2, cp);
				if (flag == 1 && flg == 0) {
					flag2 = 1;
					if (cp == cp3) {
						if (yes[i] >= 0) {
							cpu = i;
							ban[check_i3][check_j3] = 0;
							ban[check_i2][check_j2] = 0;
							return 1;
						}
					}
					else {
						if (not[i] >= 0) {
							cpu = i;
							ban[check_i3][check_j3] = 0;
							ban[check_i2][check_j2] = 0;
							return 1;
						}
					}
				}
			}

			//***駒を置いた場所から左上～右下3マスを調べる***
			if (flag == 0) {
				flag = check_LURD(check_i2, check_j2, cp);
				if (flag == 1) {
					flag2 = 1;
				}
			}

			//***駒を置いた場所から右上～左下3マスを調べる***
			if (flag == 0) {
				flag = check_RULD(check_i2, check_j2, cp);
				if (flag == 1) {
					flag2 = 1;
				}
			}

			ban[check_i3][check_j3] = 0;

			if (cp == cp3) {
				if (flag == 1 && yes_check_wreach[i] >= 0 && flg == 0) {
					cpu = check_j2;
					ban[check_i2][check_j2] = 0;
					return 1;
				}
			}
			else {
				if (flag == 1 && not_check_wreach[i] >= 0 && flg == 0) {
					cpu = check_j2;
					ban[check_i2][check_j2] = 0;
					return 1;
				}
			}

			if (flag2 == 1) {
				flag3++;
			}
			if (flag3 > 1) {
				ban[check_i2][check_j2] = 0;
				if (cp == cp3) {
					if (not2[check_j2] == -1) {
						cpu = check_j2;
						return 1;
					}
				}
				if (cp != cp3) {
					if (yes2[check_j2] == -1) {
						cpu = check_j2;
						return 1;
					}
				}
			}
			flag2 = 0;
		}	    //for文kの終わり
		flag3 = 0;
		flag2 = 0;
		flg = 0;
		x_s2 = -3;
		x_e2 = 3;
		ban[check_i2][check_j2] = 0;
	}  //for文iの終わり
	return 0;
}

int vic_cp(int cp) {

	int i, c = 0, k = 0, p = 0, y = -3, x = 0, flag = 0, flag2 = 0, flag3 = 0, flag4 = 0, flag5 = 0, roop = 1, lose = 0;
	int check_i, check_j, kouho = -1, checks[144] = { 0 };
	int stack1[288] = { 0 };
	int ch_i[288], ch_j[288];
	int ch_i2[288], ch_j2[288];
	int ban2[12][12] = { 0 };
	int flg = 0;

	memcpy(ban2, ban, sizeof(ban));
tes:
	if (cp == cp3) {
		check_eval(cp);
	}
	else {
		check_eval(sw(cp));
	}
	//*1
	for (i = 0; i < 12; i++) {
	tes3:
		if (flg == 1) {
			i = checks[roop - 1];
		}

		if (cp == cp3) {
			while (1) {
				if (not[i] >= 0 || (not2[i] >= 0 && checks[roop - 1] != i)) {
					i++;
					if (i > 11) {
						goto tes2;
					}
				}
				else {
					break;
				}
			}
		}
		else {
			while (1) {
				if (yes[i] >= 0 || (yes2[i] >= 0 && checks[roop - 1] != i)) {
					i++;
					if (i > 11) {
						goto tes2;
					}
				}
				else {
					break;
				}
			}
		}

		p = top(cp, i, &check_i, &check_j);
		if (p == 1) {
			i++;
			if (i > 11) {
				goto tes2;
			}
			if (flg == 0) {
				goto tes3;
			}
			else {
				goto tes2;
			}
		}

		if (disp2 == 1) {
			display();  ///////////////
		}
		flag = check_v(check_i, check_j, cp);

		if (flag == 1) {
			stack1[12 * roop - 12 + i] = 1;
		}
		if (flg == 1) { //自分がリーチできる場所が無かったということ（相手のリーチを防いで終わり）
			flag = check(check_i, check_j, cp);
			if (flag == 1) {
				goto tes_re;
			}
			ban[check_i][check_j] = 0;
			if (disp2 == 1) {
				display();  ///////////////
			}
			break;
		}
		ban[check_i][check_j] = 0;
		if (disp2 == 1) {
			display();  ///////////////
		}
	}
tes2:
	flg = 0;
	checks[roop - 1] = -1;
	//*2
	for (i = 0; i < 12; i++) {
		if (stack1[12 * roop - 12 + i] == 1) {
			p = top(cp, i, &ch_i[roop], &ch_j[roop]);
			dep_count++;
			if (disp2 == 1) {
				display();  ///////////////
			}
			stack1[12 * roop - 12 + i] = 0;
			flag2 = 1;
			if (roop == 1) {
				kouho = i;
			}
			break;
		}
	}
	if (flag2 == 0) {
		if (roop != 1) {
			roop = roop - 1;
			ban[ch_i2[roop]][ch_j2[roop]] = 0;
			ban[ch_i[roop]][ch_j[roop]] = 0;
			dep_count = dep_count - 2;
			if (disp2 == 1) {
				display();  ///////////////
			}
			goto tes2;
		}
		else {
			return 0;
		}
	}
	flag2 = 0;

	//*3
	check_v(ch_i[roop], ch_j[roop], cp);
	top(sw(cp), cpu, &ch_i2[roop], &ch_j2[roop]);
	dep_count++;
	if (disp2 == 1) {
		display();  ///////////////
	}


	flag4 = check_v(ch_i2[roop], ch_j2[roop], sw(cp));
	if (flag4 == 1) {
		checks[roop] = cpu;
		flg = 1;
	}

	//*4
	flag = 0;
	if (flg == 0) {
		if (check_wreach(cp) == 1) {
			if (cp == cp3 && not[cpu] == -1) {
				flag = 1;
			}
			else if (cp != cp3 && yes[cpu] == -1) {
				flag = 1;
			}
		}
	}
	else if (flg == 1) {
		if (check_wreach(cp) == 1) {
			if (cpu == checks[roop]) {
				flag = 1;
			}
		}
	}

	if (flag == 1) {
	tes_re:
		cpu = kouho;
		dep_count = dep_count + 3;
		if (disp2 == 1) {
			display();  ///////////////
		}
		memcpy(ban, ban2, sizeof(ban2));
		return 1;
	}
	roop++;
	//	printf("roop = %d\n",roop);
	goto tes;
	return 0;
}

double eval1(int check_i, int check_j) {
	int i, j, flag = 0, count = 0, reach;
	double eval = 0, oth = 0;
	//  eval = (rand() % 8000);

	for (i = 11; i >= ie; i--) {
		for (j = 0; j < 9; j++) {
			//---o
			//先手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 1) {
				eval = eval + oth + p1 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p1 * ab1;
			}

			//--o-
			//先手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p2 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p2 * ab1;
			}

			//--oo
			//先手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 1) {
				eval = eval + oth + p3 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 1) {
				eval = eval + oth + p3 * ab1;
			}

			//-o--
			//先手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p4 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p4 * ab1;
			}

			//-o-o
			//先手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 1) {
				eval = eval + oth + p5 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p5 * ab1;
			}

			//-oo-
			//先手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p6 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p6 * ab1;
			}

			//-ooo
			//先手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 1) {
				eval = eval + oth + p7 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 1) {
				eval = eval + oth + p7 * ab1;
			}

			//o---
			//先手
			//右上～左下
			if (ban[i][j] == 1 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p8 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p8 * ab1;
			}

			//o--o
			//先手
			//右上～左下
			if (ban[i][j] == 1 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 1) {
				eval = eval + oth + p9 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p9 * ab1;
			}

			//o-o-
			//先手
			//右上～左下
			if (ban[i][j] == 1 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p10 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p10 * ab1;
			}

			//o-oo
			//先手
			//右上～左下
			if (ban[i][j] == 1 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 1) {
				eval = eval + oth + p11 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 1) {
				eval = eval + oth + p11 * ab1;
			}

			//oo--
			//先手
			//右上～左下
			if (ban[i][j] == 1 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p12 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p12 * ab1;
			}

			//oo-o
			//先手
			//右上～左下
			if (ban[i][j] == 1 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 1) {
				eval = eval + oth + p13 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p13 * ab1;
			}

			//ooo-
			//先手
			//右上～左下
			if (ban[i][j] == 1 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p14 * ab1;
			}
			//左上～右下
			if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p14 * ab1;
			}

			//oooo
			//先手
			//右上～左下
			//if (ban[i][j] == 1 && ban[i - 1][j + 1] == 1 && ban[i - 2][j + 2] == 1 && ban[i - 3][j + 3] == 1) {
			//	eval = eval + oth + p15 * ab1;
			//}
			////左上～右下
			//if (ban[i - 3][j] == 1 && ban[i - 2][j + 1] == 1 && ban[i - 1][j + 2] == 1 && ban[i][j + 3] == 1) {
			//	eval = eval + oth + p15 * ab1;
			//}
		}
	}

	for (i = 11; i >= ie; i--) {
		for (j = 0; j < 12; j++) {
			//縦
			if (ban[i][j] == 1 && ban[i - 1][j] == 0 && ban[i - 2][j] == 0 && ban[i - 3][j] == 0) {
				eval = eval + oth + p8 * ab1;
			}
			//縦
			if (ban[i][j] == 1 && ban[i - 1][j] == 1 && ban[i - 2][j] == 0 && ban[i - 3][j] == 0) {
				eval = eval + oth + p12 * ab1;
			}
			//縦
			if (ban[i][j] == 1 && ban[i - 1][j] == 1 && ban[i - 2][j] == 1 && ban[i - 3][j] == 0) {
				eval = eval + oth + p14 * ab1;
			}
		}
	}

	for (i = 11; i >= ie; i--) {
		for (j = 0; j < 9; j++) {
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 0 && ban[i][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p1 * ab1;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 0 && ban[i][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p2 * ab1;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 0 && ban[i][j + 2] == 1 && ban[i][j + 3] == 1) {
				eval = eval + oth + p3 * ab1;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 1 && ban[i][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p4 * ab1;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 1 && ban[i][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p5 * ab1;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 1 && ban[i][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p6 * ab1;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 1 && ban[i][j + 2] == 1 && ban[i][j + 3] == 1) {
				eval = eval + oth + p7 * ab1;
			}
			//横
			if (ban[i][j] == 1 && ban[i][j + 1] == 0 && ban[i][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p8 * ab1;
			}
			//横
			if (ban[i][j] == 1 && ban[i][j + 1] == 0 && ban[i][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p9 * ab1;
			}
			//横
			if (ban[i][j] == 1 && ban[i][j + 1] == 0 && ban[i][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p10 * ab1;
			}
			//横
			if (ban[i][j] == 1 && ban[i][j + 1] == 0 && ban[i][j + 2] == 1 && ban[i][j + 3] == 1) {
				eval = eval + oth + p11 * ab1;
			}
			//横
			if (ban[i][j] == 1 && ban[i][j + 1] == 1 && ban[i][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p12 * ab1;
			}
			//横
			if (ban[i][j] == 1 && ban[i][j + 1] == 1 && ban[i][j + 2] == 0 && ban[i][j + 3] == 1) {
				eval = eval + oth + p13 * ab1;
			}
			//横
			if (ban[i][j] == 1 && ban[i][j + 1] == 1 && ban[i][j + 2] == 1 && ban[i][j + 3] == 0) {
				eval = eval + oth + p14 * ab1;
			}
			//横
			//if (ban[i][j] == 1 && ban[i][j + 1] == 1 && ban[i][j + 2] == 1 && ban[i][j + 3] == 1) {
			//	eval = eval + oth + p15 * ab1;
			//}
		}
	}

	flag = check_wreach_mini(1);
	if (flag == 1) {
		eval = eval + p16 * ab1;
	}

	if (trapreach_flag == 1) {
		//	display();
		eval = eval + p17 * ab1;
	}

	if (check_air_wreach_1_flag == 1) {
//		display();
		eval = eval + p18 * ab1;
	}
	//cp_flag1 = 0;
	//reach = check_v3(check_i, check_j, 1);
	//if (reach == -1) {
	//	flag = vic_cp(1);
	//	if (flag == 1) {
	//	//		display();
	//		eval = eval + p19 * ab1;
	//		cp_flag1 = 1;
	//	}
	//}

	return eval;
}

double eval2(int check_i, int check_j) {
	int i, j, flag = 0, count = 0, reach;
	double eval = 0, oth = 0;
	//  eval = (rand() % 8000);

	for (i = 11; i >= ie; i--) {
		for (j = 0; j < 9; j++) {
			//---o
			//後手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 2) {
				eval = eval + oth + p1 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p1 * ab2;
			}
			//--o-
			//後手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p2 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p2 * ab2;
			}

			//--oo
			//後手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 2) {
				eval = eval + oth + p3 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 2) {
				eval = eval + oth + p3 * ab2;
			}

			//-o--
			//後手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p4 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p4 * ab2;
			}

			//-o-o
			//後手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 2) {
				eval = eval + oth + p5 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p5 * ab2;
			}

			//-oo-
			//後手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p6 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p6 * ab2;
			}

			//-ooo
			//後手
			//右上～左下
			if (ban[i][j] == 0 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 2) {
				eval = eval + oth + p7 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 0 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 2) {
				eval = eval + oth + p7 * ab2;
			}

			//o---
			//後手
			//右上～左下
			if (ban[i][j] == 2 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p8 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p8 * ab2;
			}

			//o--o
			//後手
			//右上～左下
			if (ban[i][j] == 2 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 2) {
				eval = eval + oth + p9 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p9 * ab2;
			}

			//o-o-
			//後手
			//右上～左下
			if (ban[i][j] == 2 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p10 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p10 * ab2;
			}

			//o-oo
			//後手
			//右上～左下
			if (ban[i][j] == 2 && ban[i - 1][j + 1] == 0 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 2) {
				eval = eval + oth + p11 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 0 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 2) {
				eval = eval + oth + p11 * ab2;
			}

			//oo--
			//後手
			//右上～左下
			if (ban[i][j] == 2 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p12 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p12 * ab2;
			}

			//oo-o
			//後手
			//右上～左下
			if (ban[i][j] == 2 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 0 && ban[i - 3][j + 3] == 2) {
				eval = eval + oth + p13 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p13 * ab2;
			}

			//ooo-
			//後手
			//右上～左下
			if (ban[i][j] == 2 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 0) {
				eval = eval + oth + p14 * ab2;
			}
			//左上～右下
			if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p14 * ab2;
			}

			//oooo
			//後手
			//右上～左下
			//if (ban[i][j] == 2 && ban[i - 1][j + 1] == 2 && ban[i - 2][j + 2] == 2 && ban[i - 3][j + 3] == 2) {
			//	eval = eval + oth + p15 * ab2;
			//}
			////左上～右下
			//if (ban[i - 3][j] == 2 && ban[i - 2][j + 1] == 2 && ban[i - 1][j + 2] == 2 && ban[i][j + 3] == 2) {
			//	eval = eval + oth + p15 * ab2;
			//}
		}
	}

	for (i = 11; i >= ie; i--) {
		for (j = 0; j < 12; j++) {
			//縦
			if (ban[i][j] == 2 && ban[i - 1][j] == 0 && ban[i - 2][j] == 0 && ban[i - 3][j] == 0) {
				eval = eval + oth + p8 * ab2;
			}
			//縦
			if (ban[i][j] == 2 && ban[i - 1][j] == 2 && ban[i - 2][j] == 0 && ban[i - 3][j] == 0) {
				eval = eval + oth + p12 * ab2;
			}
			//縦
			if (ban[i][j] == 2 && ban[i - 1][j] == 2 && ban[i - 2][j] == 2 && ban[i - 3][j] == 0) {
				eval = eval + oth + p14 * ab2;
			}
		}
	}

	for (i = 11; i >= ie; i--) {
		for (j = 0; j < 9; j++) {
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 0 && ban[i][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p1 * ab2;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 0 && ban[i][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p2 * ab2;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 0 && ban[i][j + 2] == 2 && ban[i][j + 3] == 2) {
				eval = eval + oth + p3 * ab2;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 2 && ban[i][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p4 * ab2;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 2 && ban[i][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p5 * ab2;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 2 && ban[i][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p6 * ab2;
			}
			//横
			if (ban[i][j] == 0 && ban[i][j + 1] == 2 && ban[i][j + 2] == 2 && ban[i][j + 3] == 2) {
				eval = eval + oth + p7 * ab2;
			}
			//横
			if (ban[i][j] == 2 && ban[i][j + 1] == 0 && ban[i][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p8 * ab2;
			}
			//横
			if (ban[i][j] == 2 && ban[i][j + 1] == 0 && ban[i][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p9 * ab2;
			}
			//横
			if (ban[i][j] == 2 && ban[i][j + 1] == 0 && ban[i][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p10 * ab2;
			}
			//横
			if (ban[i][j] == 2 && ban[i][j + 1] == 0 && ban[i][j + 2] == 2 && ban[i][j + 3] == 2) {
				eval = eval + oth + p11 * ab2;
			}
			//横
			if (ban[i][j] == 2 && ban[i][j + 1] == 2 && ban[i][j + 2] == 0 && ban[i][j + 3] == 0) {
				eval = eval + oth + p12 * ab2;
			}
			//横
			if (ban[i][j] == 2 && ban[i][j + 1] == 2 && ban[i][j + 2] == 0 && ban[i][j + 3] == 2) {
				eval = eval + oth + p13 * ab2;
			}
			//横
			if (ban[i][j] == 2 && ban[i][j + 1] == 2 && ban[i][j + 2] == 2 && ban[i][j + 3] == 0) {
				eval = eval + oth + p14 * ab2;
			}
			//横
			//if (ban[i][j] == 2 && ban[i][j + 1] == 2 && ban[i][j + 2] == 2 && ban[i][j + 3] == 2) {
			//	eval = eval + oth + p15 * ab2;
			//}
		}
	}

	flag = check_wreach_mini(2);
	if (flag == 1) {
		eval = eval + p16 * ab2;
	}

	if (trapreach_flag == 2) {
		//display();
		eval = eval + p17 * ab2;
	}

	if (check_air_wreach_2_flag == 1) {
		eval = eval + p18 * ab2;
	}
	//cp_flag2 = 0;
	//reach = check_v3(check_i, check_j, 2);
	//if (reach == -1) {
	//	flag = vic_cp(2);
	//	if (flag == 1) {
	//	//	display();
	//		eval = eval + p19 * ab2;
	//		cp_flag2 = 1;
	//	}
	//}

	return eval;
}

int may_vic(int cp) {
	int i, flag = 0, p, kouho1 = -1, check = 0;
	int check_i, check_j;

	for (i = 0; i < 12; i++) {

		while (1) {
			if (cp == cp3) {
				while (1) {
					if (not[i] >= 0 || not2[i] >= 0) {
						i++;
						if (i > 11) {
							break;
						}
					}
					else {
						break;
					}
				}
			}
			else {
				while (1) {
					if (yes[i] >= 0 || yes2[i] >= 0) {
						i++;
						if (i > 11) {
							break;
						}
					}
					else {
						break;
					}
				}
			}
			if (i > 11) {
				break;
			}
			p = top(cp, i, &check_i, &check_j);
			if (p == 1) {
				i++;
				if (i > 11) {
					break;
				}
			}
			if (check_v(check_i, check_j, cp) == 0) {
				break;
			}
			else {
				ban[check_i][check_j] = 0;
				i++;
			}
		}
		if (i > 11) {
			break;
		}

		check_evalx(sw(cp), i);

		if (yes[i] >= 0) {
			flag = vic_cp(cp);
		}

		if (flag == 1) {
			ban[check_i][check_j] = 0;
			return 1;
		}

		ban[check_i][check_j] = 0;
	}

	return 0;
}

int trapreach(int cp) {
	int i, j, flag = 0, flag2 = 0, fst_odd = 0, fst_even = 0, scd_odd = 0, scd_even = 0, retsu_sum, te, ban_queue = 11, check_rc[12];
	int fst_same = 0, scd_same = 0;

	//初期化
	for (i = 0; i <= 11; i++) {
		check_rc[i] = -1;
	}

trap:

	if (cp == 1) {

		if (flag == 0) {
			for (i = 0; i < 12; i++) {
				if (yes[i] != -1 && not[i] != -1 && yes[i] == not[i]) {
					fst_same++;
					check_rc[i] = yes[i];
					break;
				}
				if (yes[i] != -1 && yes[i] % 2 == 1) {
					fst_odd++;
					check_rc[i] = yes[i];
				}
				else if (yes[i] != -1) {
					fst_even++;
					check_rc[i] = yes[i];
				}
				if (not[i] != -1 && not2[i] != 88 && not[i] % 2 == 1) {
					scd_odd++;
					check_rc[i] = not[i];
				}
				else if (not[i] != -1 && not2[i] != 88) {
					scd_even++;
					check_rc[i] = not[i];
				}
			}
		}
		flag == 1;

		for (i = 0; i < 12; i++) {

			if (check_rc[i] == -1) {
				ban_queue = 10;
			}
			else {
				ban_queue = check_rc[i] - 1;
			}

			if (flag2 == 1) {
				//初期化
				for (j = 0; j <= 11; j++) {
					check_rc[i] = -1;
				}
			}

			for (j = ban_queue; j >= 0; j--) {
				if (ban_1[j][i] == 1 && ban_2[j][i] == 0 && check_rc[i] == -1 && j % 2 == 1) {
					fst_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 1 && ban_2[j][i] == 0 && check_rc[i] == -1) {
					fst_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 0 && ban_2[j][i] == 1 && check_rc[i] == -1 && j % 2 == 1) {
					scd_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 0 && ban_2[j][i] == 1 && check_rc[i] == -1) {
					scd_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 1 && ban_2[j][i] == 1 && check_rc[i] == -1) {
					scd_odd++;
					check_rc[i] = j;
					break;
				}
				if (j == 0 && check_rc[i] == -1) {
					check_rc[i] = 0;
				}
			}
		}
	}
	else {

		if (flag == 0) {
			for (i = 0; i < 12; i++) {
				if (yes[i] != -1 && not[i] != -1 && yes[i] == not[i]) {
					scd_same++;
					check_rc[i] = not[i];
					break;
				}
				if (not[i] != -1 && not2[i] != 88 && not[i] % 2 == 1) {
					fst_odd++;
					check_rc[i] = 1;
				}
				else if (not[i] != -1 && not2[i] != 88) {
					fst_even++;
					check_rc[i] = 1;
				}
				if (yes[i] != -1 && yes[i] % 2 == 1) {
					scd_odd++;
					check_rc[i] = 1;
				}
				else if (yes[i] != -1) {
					scd_even++;
					check_rc[i] = 1;
				}
			}
		}
		flag == 1;

		for (i = 0; i < 12; i++) {

			if (check_rc[i] == -1) {
				ban_queue = 10;
			}
			else {
				ban_queue = check_rc[i] - 1;
			}

			if (flag2 == 1) {
				//初期化
				for (j = 0; j <= 11; j++) {
					check_rc[i] = -1;
				}
			}

			for (j = ban_queue; j >= 0; j--) {
				if (ban_1[j][i] == 1 && ban_2[j][i] == 0 && check_rc[i] == -1 && j % 2 == 1) {
					scd_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 1 && ban_2[j][i] == 0 && check_rc[i] == -1) {
					scd_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 0 && ban_2[j][i] == 1 && check_rc[i] == -1 && j % 2 == 1) {
					fst_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 0 && ban_2[j][i] == 1 && check_rc[i] == -1) {
					fst_odd++;
					check_rc[i] = j;
					break;
				}
				if (ban_1[j][i] == 1 && ban_2[j][i] == 1 && check_rc[i] == -1) {
					fst_odd++;
					check_rc[i] = j;
					break;
				}
				if (j == 0 && check_rc[i] == -1) {
					check_rc[i] = 0;
				}
			}
		}
	}

	retsu_sum = fst_odd + fst_even + scd_odd + scd_even + fst_same + scd_same;

	if (retsu_sum == 0) {
		return 0;
	}

	if ((fst_odd + scd_odd + fst_same + scd_same) % 2 == 0) {
		te = 1;
	}
	else {
		te = 2;
	}

	while (1) {
		if (te == 1) {
			if (fst_odd > 0) {
				fst_odd--;
				retsu_sum--;
				te = 3 - te;
			}
			else if (fst_even > 0) {
				fst_even--;
				retsu_sum--;
			}
			else if (scd_odd > 0) {
				return 2;
			}
			else if (scd_even > 0) {
				return 2;
			}
			else if (fst_same > 0) {
				return 2;
			}
			else if (scd_same > 0) {
				return 2;
			}
			if (retsu_sum == 0) {
				if (ban_queue >= 0 && ban_queue <= 10) {
					flag = 1;
					flag2 = 1;
					goto trap;
				}
				return 0;
			}
		}

		if (te == 2) {
			if (scd_odd > 0) {
				scd_odd--;
				retsu_sum--;
				te = 3 - te;
			}
			else if (scd_even > 0) {
				scd_even--;
				retsu_sum--;
			}
			else if (fst_odd > 0) {
				return 1;
			}
			else if (fst_even > 0) {
				return 1;
			}
			else if (fst_same > 0) {
				return 1;
			}
			else if (scd_same > 0) {
				return 1;
			}
			if (retsu_sum == 0) {
				if (ban_queue >= 0 && ban_queue <= 10) {
					flag = 1;
					flag2 = 1;
					goto trap;
				}
				return 0;
			}
		}
	}

	return 0;
}

int check_air_wreach(int cp) {
	int i, j, k, flag, count = 0, air_wreach_flag = 0;

		for (i = 0; i <= 11; i++) {
			for (j = 11; j >= 0; j--) {
				if (ban[j][i] == 0) {
					ban[j][i] = cp;
						//display();
					//***駒を置いた左右3マスを調べる***
					flag = check_LR(j, i, cp);

					//***駒を置いた場所から上下3マスを調べる***
					/*if (flag == 0) {
					flag = check_UD(check_i + i, check_j - 3, cp);
					}*/

					//***駒を置いた場所から左上～右下3マスを調べる***
					if (flag == 0) {
						flag = check_LURD(j, i, cp);
					}

					//***駒を置いた場所から右上～左下3マスを調べる***
					if (flag == 0) {
						flag = check_RULD(j, i, cp);
					}
					if (flag == 1) {
						if(j + 1 <= 11 && ban[j + 1][i] == 0){
						if (cp == 1) {
							if (cp == cp3) {
								if (yes[i] != j) {
									ban_1[j][i] = 1;
								}
							}
							else {
								if (not[i] != j) {
									ban_1[j][i] = 1;
								}
							}
						}
						else {
							if (cp == cp3) {
								if (yes[i] != j) {
									ban_2[j][i] = 1;
								}
							}
							else {
								if (not[i] != j) {
									ban_2[j][i] = 1;
								}
							}
						}
						count++;
						if (count == 2) {
							if (cp == 1) {
								if (cp == cp3) {
									if (not[i] != j + 1) {
										k = 1;
										while (ban[j + k + 1][i] == 0 && j + k + 1 <= 11) {
											if (ban_2[j + k][i] == 1) {
												count = 0;
												goto air;
											}
											k++;
										}
										air_wreach_flag = 1;
									}
								}
								else {
									if (yes[i] != j + 1) {
										k = 1;
										while (ban[j + k + 1][i] == 0 && j + k + 1 <= 11) {
											if (ban_2[j + k][i] == 1) {
												count = 0;
												goto air;
											}
											k++;
										}
										air_wreach_flag = 1;
									}
								}
							}
							else {
								if (cp == cp3) {
									if (not[i] != j + 1) {
										k = 1;
										while (ban[j + k + 1][i] == 0 && j + k + 1 <= 11) {
											if (ban_1[j + k][i] == 1) {
												count = 0;
												goto air;
											}
											k++;
										}
										air_wreach_flag = 1;
									}
								}
								else {
									if (yes[i] != j + 1) {
										k = 1;
										while (ban[j + k + 1][i] == 0 && j + k + 1 <= 11) {
											if (ban_1[j + k][i] == 1) {
												count = 0;
												goto air;
											}
											k++;
										}
										air_wreach_flag = 1;
									}
								}
							}
						}
					}
					}
					else {
						count = 0;
						if (cp == 1) {
							ban_1[j][i] = 0;
						}
						else {
							ban_2[j][i] = 0;
						}
					}
					air:
					ban[j][i] = 0;
				}
			}
		}

	return air_wreach_flag;
}

//2手先まで
int minimax(int cp) {
	int i, j, k, l, m, p, check_4 = 0, count = 0;
	int check_i, check_j, check_i2, check_j2, reach_1;
	double eval_temp = 0, eval_sum, eval_max1 = -INFINITY, eval_min2;

	check_eval(cp);
	check_eval(sw(cp));

	//初期化
	for (i = 0; i < 12; i++) {
		node_max[i] = -INFINITY;
	}

	for (i = 0; i < 12; i++) { // 1手先
	mini1:

		eval_min2 = INFINITY;
		check_evalx(1, i);
		check_evalx(2, i);

		if (cp == cp3) {
			if (not[i] >= 0 || not2[i] >= 0) {
				i++;
				if (i > 11) {
					break;
				}
				goto mini1;
			}
		}
		else {
			if (yes[i] >= 0 || yes2[i] >= 0) {
				i++;
				if (i > 11) {
					break;
				}
				goto mini1;
			}
		}

		p = top(cp, i, &check_i, &check_j);
		if (disp2 == 1) {
			display();  ///////////////
		}
		if (p == 1) {
			i++;
			if (i > 11) {
				break;
			}
			goto mini1;
		}

		check_4 = check(check_i, check_j, cp);

		if (check_4 == 0) {
			reach_1 = check_v3(check_i, check_j, cp);
		}
		else {
			reach_1 = -1;
		}

			//ooooooooooooooooooooooooo
			for (j = 0; j < 12; j++) { // 2手先
			mini2:

				check_eval2(check_i, j, 1);
				check_eval2(check_i, j, 2);

				if (cp == cp3) {
					if (yes[j] >= 0 || (reach_1 != -1 && reach_1 != j)) {
						j++;
						if (j > 11) {
							break;
						}
						goto mini2;
					}
				}
				else {
					if (not[j] >= 0 || (reach_1 != -1 && reach_1 != j)) {
						j++;
						if (j > 11) {
							break;
						}
						goto mini2;
					}
				}

				p = top(sw(cp), j, &check_i2, &check_j2);
				if (disp2 == 1) {
					display();  ///////////////
				}
				if (p == 1) {
					j++;
					if (j > 11) {
						break;
					}
					goto mini2;
				}

				memset(ban_1, 0, sizeof(ban_1));
				memset(ban_2, 0, sizeof(ban_2));

				//空中にあるtrapreach＆空中Wreachを調べる
				check_air_wreach_1_flag = check_air_wreach(1);
				check_air_wreach_2_flag = check_air_wreach(2);

				//trapreachを調べる
				trapreach_flag = trapreach(cp);

				//-------------------------
				ie = 3;
				for (m = je; m >= 3; m--) {
					if (ban[m][0] == 0 && ban[m][1] == 0 && ban[m][2] == 0 && ban[m][3] == 0 && ban[m][4] == 0 && ban[m][5] == 0 && ban[m][6] == 0 && ban[m][7] == 0 && ban[m][8] == 0 && ban[m][9] == 0 && ban[m][10] == 0 && ban[m][11] == 0) {
						ie = m;
						break;
					}
				}

				eval_sum = eval1(check_i, check_j) + eval2(check_i, check_j); //現在の局面を評価

				if (cp_flag1 == 1 && cp_flag2 == 1) { //vic_cpが先手後手両方1の場合、自分の評価値は取り消す
					eval_sum = eval_sum - p19;
				}

				ban[check_i2][check_j2] = 0; //配置戻し
				count++; //eval関数を呼び出した回数をカウントする

				if (eval_sum < eval_min2) {
					eval_min2 = eval_sum;
				}
				//-------------------------

			}
			//ooooooooooooooooooooooooo
			ban[check_i][check_j] = 0; //配置戻し

			if (eval_min2 == INFINITY) {
				eval_min2 = -INFINITY;
			}

			eval_sum = eval_min2;
			node_max[12 * 1 - 12 + i] = eval_min2;
			if (eval_sum > eval_max1) {
				eval_max1 = eval_sum;
				cpu = i;
			}
			if (i <= 9) {
				printf("node_max[%d] = %5.1f\n", i, node_max[i]);
			}
			else if (i == 10) {
				printf("node_max[A] = %5.1f\n", node_max[i]);
			}
			else {
				printf("node_max[B] = %5.1f\n", node_max[i]);
			}
	}

	printf("評価回数: %d\n", count);
	return 0;
}

//3手先まで
int alphabeta3(int cp) {
	int i, depth1, depth2, depth3, m, p, check_4 = 0, count = 0;
	int check_i, check_j, check_i2, check_j2, check_i3, check_j3, reach_1, reach_2;
	double eval_temp = 0, eval_sum, eval_max1 = -INFINITY, eval_min2, eval_max3;

	check_eval(cp);
	check_eval(sw(cp));

	//初期化
	for (i = 0; i < 12; i++) {
		node_max[i] = -INFINITY;
	}

	for (depth1 = 0; depth1 < 12; depth1++) { // 1手先
	mini1:

		eval_min2 = INFINITY;
		check_evalx(1, eval_sort[depth1]);
		check_evalx(2, eval_sort[depth1]);

		if (cp == cp3) {
			if (not[eval_sort[depth1]] >= 0 || not2[eval_sort[depth1]] >= 0) {
				depth1++;
				if (depth1 > 11) {
					break;
				}
				goto mini1;
			}
		}
		else {
			if (yes[eval_sort[depth1]] >= 0 || yes2[eval_sort[depth1]] >= 0) {
				depth1++;
				if (depth1 > 11) {
					break;
				}
				goto mini1;
			}
		}

		p = top(cp, eval_sort[depth1], &check_i, &check_j);
		if (disp2 == 1) {
			display();  ///////////////
		}
		if (p == 1) {
			depth1++;
			if (depth1 > 11) {
				break;
			}
			goto mini1;
		}

		check_4 = check(check_i, check_j, cp);

		if (check_4 == 0) {
			reach_1 = check_v3(check_i, check_j, cp);
		}
		else {
			reach_1 = -1;
		}

			//ooooooooooooooooooooooooo
			for (depth2 = 0; depth2 < 12; depth2++) { // 2手先
			mini2:

				check_eval2(check_i, eval_sort[depth2], 1);
				check_eval2(check_i, eval_sort[depth2], 2);

				eval_max3 = -INFINITY;

				if (cp == cp3) {
					if (yes[eval_sort[depth2]] >= 0 || (reach_1 != -1 && reach_1 != eval_sort[depth2])) {
						depth2++;
						if (depth2 > 11) {
							break;
						}
						goto mini2;
					}
				}
				else {
					if (not[eval_sort[depth2]] >= 0 || (reach_1 != -1 && reach_1 != eval_sort[depth2])) {
						depth2++;
						if (depth2 > 11) {
							break;
						}
						goto mini2;
					}
				}

				p = top(sw(cp), eval_sort[depth2], &check_i2, &check_j2);
				if (disp2 == 1) {
					display();  ///////////////
				}
				if (p == 1) {
					depth2++;
					if (depth2 > 11) {
						break;
					}
					goto mini2;
				}

				check_4 = check(check_i2, check_j2, sw(cp));

				if (check_4 == 0) {
					reach_2 = check_v3(check_i2, check_j2, sw(cp));
					if (reach_2 == -1) {
						reach_2 = check_wreach_mini(sw(cp));
					}
					else if (reach_2 == -1) {
						reach_2 = vic_cp(sw(cp));
					}
				}
				else {
					reach_2 = -1;
				}

				if (check_4 == 0) {
									 //xxxxxxxxxxxxxxxxxxxxxxxxx
					for (depth3 = 0; depth3 < 12; depth3++) { // 4手先
					mini3:

						check_eval(1);
						check_eval(2);

						if (cp == cp3) {
							if (not[eval_sort[depth3]] >= 0 || (reach_2 != -1 && reach_2 != eval_sort[depth3])) {
								depth3++;
								if (depth3 > 11) {
									break;
								}
								goto mini3;
							}
						}
						else {
							if (yes[eval_sort[depth3]] >= 0 || (reach_2 != -1 && reach_2 != eval_sort[depth3])) {
								depth3++;
								if (depth3 > 11) {
									break;
								}
								goto mini3;
							}
						}

						p = top(cp, eval_sort[depth3], &check_i3, &check_j3);
						if (disp2 == 1) {
							display();  ///////////////
						}
						if (p == 1) {
							depth3++;
							if (depth3 > 11) {
								break;
							}
							goto mini3;
						}

						check_4 = check(check_i3, check_j3, cp);

						if (check_4 == 0) { //4つ並んでいないなら評価
											//-------------------------
							ie = 3;
							for (m = je; m >= 3; m--) {
								if (ban[m][0] == 0 && ban[m][1] == 0 && ban[m][2] == 0 && ban[m][3] == 0 && ban[m][4] == 0 && ban[m][5] == 0 && ban[m][6] == 0 && ban[m][7] == 0 && ban[m][8] == 0 && ban[m][9] == 0 && ban[m][10] == 0 && ban[m][11] == 0) {
									ie = m;
									break;
								}
							}

							memset(ban_1, 0, sizeof(ban_1));
							memset(ban_2, 0, sizeof(ban_2));

							//空中にあるtrapreach＆空中Wreachを調べる
							check_air_wreach_1_flag = check_air_wreach(1);
							check_air_wreach_2_flag = check_air_wreach(2);

							//trapreachを調べる
							trapreach_flag = trapreach(cp);

							eval_sum = eval1(check_i3, check_j3) + eval2(check_i3, check_j3); //現在の局面を評価

							if (cp_flag1 == 1 && cp_flag2 == 1) { //vic_cpが先手後手両方1の場合、自分の評価値は取り消す
								eval_sum = eval_sum - p19;
							}

							ban[check_i3][check_j3] = 0; //配置戻し
							count++; //eval関数を呼び出した回数をカウントする

							if (eval_sum > eval_min2) { // 枝刈り
								eval_max3 = INFINITY;
								break;
							}

							if (eval_sum > eval_max3) {
								eval_max3 = eval_sum;
							}
							//-------------------------
						}
						else {
							ban[check_i3][check_j3] = 0; //配置戻し
						}
					}
					//xxxxxxxxxxxxxxxxxxxxxxxxx
					ban[check_i2][check_j2] = 0; //配置戻し

					if (eval_max3 == -INFINITY) {
						eval_max3 = INFINITY;
					}

					eval_sum = eval_max3;

					if (eval_sum < eval_max1) { // 枝刈り
						eval_min2 = -INFINITY;
						break;
					}

					if (eval_sum < eval_min2) {
						eval_min2 = eval_sum;
					}
				}
				else {
					ban[check_i2][check_j2] = 0; //配置戻し
				}
			}
			//ooooooooooooooooooooooooo
			ban[check_i][check_j] = 0; //配置戻し

			if (eval_min2 == INFINITY) {
				eval_min2 = -INFINITY;
			}

			eval_sum = eval_min2;
			node_max[12 * 1 - 12 + eval_sort[depth1]] = eval_min2;
			if (eval_sum > eval_max1) {
				eval_max1 = eval_sum;
				cpu = eval_sort[depth1];
			}
			if (eval_sort[depth1] <= 9) {
				printf("node_max[%d] = %5.1f\n", eval_sort[depth1], node_max[eval_sort[depth1]]);
			}
			else if (eval_sort[depth1] == 10) {
				printf("node_max[A] = %5.1f\n", node_max[eval_sort[depth1]]);
			}
			else {
				printf("node_max[B] = %5.1f\n", node_max[eval_sort[depth1]]);
			}
	}

	printf("評価回数: %d\n", count);
	return 0;
}

//4手先まで
int alphabeta4(int cp) {
	int i, depth1, depth2, depth3, depth4, m, p, check_4 = 0, count = 0;
	int check_i, check_j, check_i2, check_j2, check_i3, check_j3, check_i4, check_j4, reach_1, reach_2, reach_3;
	double eval_temp = 0, eval_sum, eval_max1 = -INFINITY, eval_min2, eval_max3, eval_min4;

	check_eval(cp);
	check_eval(sw(cp));

	//初期化
	for (i = 0; i < 12; i++) {
		node_max[i] = -INFINITY;
	}

	for (depth1 = 0; depth1 < 12; depth1++) { // 1手先
	mini1:

		eval_min2 = INFINITY;
		check_evalx(1, eval_sort[depth1]);
		check_evalx(2, eval_sort[depth1]);

		if (cp == cp3) {
			if (not[eval_sort[depth1]] >= 0 || not2[eval_sort[depth1]] >= 0) {
				depth1++;
				if (depth1 > 11) {
					break;
				}
				goto mini1;
			}
		}
		else {
			if (yes[eval_sort[depth1]] >= 0 || yes2[eval_sort[depth1]] >= 0) {
				depth1++;
				if (depth1 > 11) {
					break;
				}
				goto mini1;
			}
		}

		p = top(cp, eval_sort[depth1], &check_i, &check_j);
		if (disp2 == 1) {
			display();  ///////////////
		}
		if (p == 1) {
			depth1++;
			if (depth1 > 11) {
				break;
			}
			goto mini1;
		}

		check_4 = check(check_i, check_j, cp);

		if (check_4 == 0) {
			reach_1 = check_v3(check_i, check_j, cp);
		}
		else {
			reach_1 = -1;
		}

			//ooooooooooooooooooooooooo
			for (depth2 = 0; depth2 < 12; depth2++) { // 2手先
			mini2:

				check_eval2(check_i, eval_sort[depth2], 1);
				check_eval2(check_i, eval_sort[depth2], 2);

				eval_max3 = -INFINITY;

				if (cp == cp3) {
					if (yes[eval_sort[depth2]] >= 0 || (reach_1 != -1 && reach_1 != eval_sort[depth2])) {
						depth2++;
						if (depth2 > 11) {
							break;
						}
						goto mini2;
					}
				}
				else {
					if (not[eval_sort[depth2]] >= 0 || (reach_1 != -1 && reach_1 != eval_sort[depth2])) {
						depth2++;
						if (depth2 > 11) {
							break;
						}
						goto mini2;
					}
				}

				p = top(sw(cp), eval_sort[depth2], &check_i2, &check_j2);
				if (disp2 == 1) {
					display();  ///////////////
				}
				if (p == 1) {
					depth2++;
					if (depth2 > 11) {
						break;
					}
					goto mini2;
				}

				check_4 = check(check_i2, check_j2, sw(cp));

				if (check_4 == 0) {
					reach_2 = check_v3(check_i2, check_j2, sw(cp));
					if (reach_2 == -1) {
						reach_2 = check_wreach_mini(sw(cp));
					}
					else if (reach_2 == -1) {
						reach_2 = vic_cp(sw(cp));
					}
				}
				else {
					reach_2 = -1;
				}

				if (check_4 == 0) {
					//xxxxxxxxxxxxxxxxxxxxxxxxx
					for (depth3 = 0; depth3 < 12; depth3++) { // 3手先
					mini3:

						check_eval(1);
						check_eval(2);

						eval_min4 = INFINITY;

						if (cp == cp3) {
							if (not[eval_sort[depth3]] >= 0 || (reach_2 != -1 && reach_2 != eval_sort[depth3])) {
								depth3++;
								if (depth3 > 11) {
									break;
								}
								goto mini3;
							}
						}
						else {
							if (yes[eval_sort[depth3]] >= 0 || (reach_2 != -1 && reach_2 != eval_sort[depth3])) {
								depth3++;
								if (depth3 > 11) {
									break;
								}
								goto mini3;
							}
						}

						p = top(cp, eval_sort[depth3], &check_i3, &check_j3);
						if (disp2 == 1) {
							display();  ///////////////
						}
						if (p == 1) {
							depth3++;
							if (depth3 > 11) {
								break;
							}
							goto mini3;
						}

						//=============

						check_4 = check(check_i3, check_j3, cp);

						if (check_4 == 0) {
							reach_3 = check_v3(check_i3, check_j3, cp);
							if (reach_3 == -1) {
								reach_3 = check_wreach_mini(cp);
							}
							else if (reach_3 == -1) {
								reach_3 = vic_cp(cp);
							}
						}
						else {
							reach_3 = -1;
						}

						if (check_4 == 0) {
							//*************************
							for (depth4 = 0; depth4 < 12; depth4++) { // 4手先
							mini4:

								check_eval(1);
								check_eval(2);

								if (cp == cp3) {
									if (yes[eval_sort[depth4]] >= 0 || (reach_3 != -1 && reach_3 != eval_sort[depth4])) {
										depth4++;
										if (depth4 > 11) {
											break;
										}
										goto mini4;
									}
								}
								else {
									if (not[eval_sort[depth4]] >= 0 || (reach_3 != -1 && reach_3 != eval_sort[depth4])) {
										depth4++;
										if (depth4 > 11) {
											break;
										}
										goto mini4;
									}
								}

								p = top(sw(cp), eval_sort[depth4], &check_i4, &check_j4);
								if (disp2 == 1) {
									display();  ///////////////
								}
								if (p == 1) {
									depth4++;
									if (depth4 > 11) {
										break;
									}
									goto mini4;
								}

								check_4 = check(check_i4, check_j4, sw(cp));

								if (check_4 == 0) { //4つ並んでいないなら評価
													//-------------------------
									ie = 3;
									for (m = je; m >= 3; m--) {
										if (ban[m][0] == 0 && ban[m][1] == 0 && ban[m][2] == 0 && ban[m][3] == 0 && ban[m][4] == 0 && ban[m][5] == 0 && ban[m][6] == 0 && ban[m][7] == 0 && ban[m][8] == 0 && ban[m][9] == 0 && ban[m][10] == 0 && ban[m][11] == 0) {
											ie = m;
											break;
										}
									}

									memset(ban_1, 0, sizeof(ban_1));
									memset(ban_2, 0, sizeof(ban_2));

									//空中にあるtrapreach＆空中Wreachを調べる
									check_air_wreach_1_flag = check_air_wreach(1);
									check_air_wreach_2_flag = check_air_wreach(2);

									//trapreachを調べる
									trapreach_flag = trapreach(cp);

									eval_sum = eval1(check_i4, check_j4) + eval2(check_i4, check_j4); //現在の局面を評価

									if (cp_flag1 == 1 && cp_flag2 == 1) { //vic_cpが先手後手両方1の場合、自分の評価値は取り消す
										eval_sum = eval_sum - p19;
									}

									ban[check_i4][check_j4] = 0; //配置戻し
									count++; //eval関数を呼び出した回数をカウントする

									if (eval_sum < eval_max3) { // 枝刈り
										eval_min4 = -INFINITY;
										break;
									}

									if (eval_sum < eval_min4) {
										eval_min4 = eval_sum;
									}
									//-------------------------
								}
								else {
									ban[check_i4][check_j4] = 0; //配置戻し
								}
							}
							//*************************
							ban[check_i3][check_j3] = 0; //配置戻し

							if (eval_min4 == INFINITY) {
								eval_min4 = -INFINITY;
							}

							eval_sum = eval_min4;

							if (eval_sum > eval_min2) { // 枝刈り
								eval_max3 = INFINITY;
								break;
							}

							if (eval_sum > eval_max3) {
								eval_max3 = eval_sum;
							}
						}
						else {
							ban[check_i3][check_j3] = 0; //配置戻し
						}
					}
					//xxxxxxxxxxxxxxxxxxxxxxxxx
					ban[check_i2][check_j2] = 0; //配置戻し

					if (eval_max3 == -INFINITY) {
						eval_max3 = INFINITY;
					}

					eval_sum = eval_max3;

					if (eval_sum < eval_max1) { // 枝刈り
						eval_min2 = -INFINITY;
						break;
					}

					if (eval_sum < eval_min2) {
						eval_min2 = eval_sum;
					}
				}
				else {
					ban[check_i2][check_j2] = 0; //配置戻し
				}
			}
			//ooooooooooooooooooooooooo
			ban[check_i][check_j] = 0; //配置戻し

			if (eval_min2 == INFINITY) {
				eval_min2 = -INFINITY;
			}

			eval_sum = eval_min2;
			node_max[12 * 1 - 12 + eval_sort[depth1]] = eval_min2;
			if (eval_sum > eval_max1) {
				eval_max1 = eval_sum;
				cpu = eval_sort[depth1];
			}
			if (eval_sort[depth1] <= 9) {
				printf("node_max[%d] = %5.1f\n", eval_sort[depth1], node_max[eval_sort[depth1]]);
			}
			else if (eval_sort[depth1] == 10) {
				printf("node_max[A] = %5.1f\n", node_max[eval_sort[depth1]]);
			}
			else {
				printf("node_max[B] = %5.1f\n", node_max[eval_sort[depth1]]);
			}
	}

	printf("評価回数: %d\n", count);
	return 0;
}

//5手先まで
int alphabeta(int cp) {
	int i,j, depth1, depth2, depth3, depth4, depth5, m, p, check_4 = 0, count = 0;
	int check_i, check_j, check_i2, check_j2, check_i3, check_j3, check_i4, check_j4, check_i5, check_j5, reach_1, reach_2, reach_3, reach_4;
	double eval_temp = 0, eval_sum, eval_max1 = -INFINITY, eval_min2, eval_max3, eval_min4, eval_max5;

	check_eval(cp);
	check_eval(sw(cp));

	//初期化
	for (i = 0; i < 12; i++) {
		node_max[i] = -INFINITY;
	}

	for (depth1 = 0; depth1 < 12; depth1++) { // 1手先
	mini1:

		eval_min2 = INFINITY;
		check_evalx(1, eval_sort[depth1]);
		check_evalx(2, eval_sort[depth1]);

		if (cp == cp3) {
			if (not[eval_sort[depth1]] >= 0 || not2[eval_sort[depth1]] >= 0) {
				depth1++;
				if (depth1 > 11) {
					break;
				}
				goto mini1;
			}
		}
		else {
			if (yes[eval_sort[depth1]] >= 0 || yes2[eval_sort[depth1]] >= 0) {
				depth1++;
				if (depth1 > 11) {
					break;
				}
				goto mini1;
			}
		}

		p = top(cp, eval_sort[depth1], &check_i, &check_j);
		if (disp3 == 1) {
			display();  ///////////////
		}
		if (p == 1) {
			depth1++;
			if (depth1 > 11) {
				break;
			}
			goto mini1;
		}

		if (eval_sort[depth1] == 5) {
//			display();
		}

		check_4 = check(check_i, check_j, cp);

		if (check_4 == 0) {
			reach_1 = check_v3(check_i, check_j, cp);
		}
		else {
			reach_1 = -1;
		}

			//ooooooooooooooooooooooooo
			for (depth2 = 0; depth2 < 12; depth2++) { // 2手先
			mini2:

				if (eval_sort[depth2] == 6) {
				//	display();
				}

				check_eval2(check_i, eval_sort[depth2], 1);
				check_eval2(check_i, eval_sort[depth2], 2);

				eval_max3 = -INFINITY;

				if (cp == cp3) {
					if (yes[eval_sort[depth2]] >= 0 || (reach_1 != -1 && reach_1 != eval_sort[depth2])) {
						depth2++;
						if (depth2 > 11) {
							break;
						}
						goto mini2;
					}
				}
				else {
					if (not[eval_sort[depth2]] >= 0 || (reach_1 != -1 && reach_1 != eval_sort[depth2])) {
						depth2++;
						if (depth2 > 11) {
							break;
						}
						goto mini2;
					}
				}

				p = top(sw(cp), eval_sort[depth2], &check_i2, &check_j2);
				if (disp3 == 1) {
					display();  ///////////////
				}
				if (p == 1) {
					depth2++;
					if (depth2 > 11) {
						break;
					}
					goto mini2;
				}

				check_4 = check(check_i2, check_j2, sw(cp));

				if (check_4 == 0) {
					reach_2 = check_v3(check_i2, check_j2, sw(cp));
					if (reach_2 == -1) {
						reach_2 = check_wreach_mini(sw(cp));
					}
					else if (reach_2 == -1) {
					//	reach_2 = vic_cp(sw(cp));
					}
				}
				else {
					reach_2 = -1;
				}

				if (check_4 == 0) {  //4つ並んでいないなら評価
									 //xxxxxxxxxxxxxxxxxxxxxxxxx
					for (depth3 = 0; depth3 < 12; depth3++) { // 3手先
					mini3:

						check_eval(1);
						check_eval(2);

						eval_min4 = INFINITY;

						if (cp == cp3) {
							if (not[eval_sort[depth3]] >= 0 || (reach_2 != -1 && reach_2 != eval_sort[depth3])) {
								depth3++;
								if (depth3 > 11) {
									break;
								}
								goto mini3;
							}
						}
						else {
							if (yes[eval_sort[depth3]] >= 0 || (reach_2 != -1 && reach_2 != eval_sort[depth3])) {
								depth3++;
								if (depth3 > 11) {
									break;
								}
								goto mini3;
							}
						}

						p = top(cp, eval_sort[depth3], &check_i3, &check_j3);
						if (disp3 == 1) {
							display();  ///////////////
						}
						if (p == 1) {
							depth3++;
							if (depth3 > 11) {
								break;
							}
							goto mini3;
						}

						//=============

						check_4 = check(check_i3, check_j3, cp);

						if (check_4 == 0) {
							reach_3 = check_v3(check_i3, check_j3, cp);
							if (reach_3 == -1) {
								reach_3 = check_wreach_mini(cp);
							}
							else if (reach_3 == -1) {
						//		reach_3 = vic_cp(cp);
							}
						}
						else {
							reach_3 = -1;
						}

						if (check_4 == 0) {
							//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
							for (depth4 = 0; depth4 < 12; depth4++) { // 4手先
							mini4:

								check_eval(1);
								check_eval(2);

								eval_max5 = -INFINITY;

								if (cp == cp3) {
									if (yes[eval_sort[depth4]] >= 0 || (reach_3 != -1 && reach_3 != eval_sort[depth4])) {
										depth4++;
										if (depth4 > 11) {
											break;
										}
										goto mini4;
									}
								}
								else {
									if (not[eval_sort[depth4]] >= 0 || (reach_3 != -1 && reach_3 != eval_sort[depth4])) {
										depth4++;
										if (depth4 > 11) {
											break;
										}
										goto mini4;
									}
								}

								p = top(sw(cp), eval_sort[depth4], &check_i4, &check_j4);
								if (disp3 == 1) {
									display();  ///////////////
								}
								if (p == 1) {
									depth4++;
									if (depth4 > 11) {
										break;
									}
									goto mini4;
								}

								//=============

								check_4 = check(check_i4, check_j4, sw(cp));
								if (check_4 == 0) {
									reach_4 = check_v3(check_i4, check_j4, sw(cp));
									if (reach_4 == -1) {
										reach_4 = check_wreach_mini(sw(cp));
									}
									else if (reach_4 == -1) {
							//			reach_4 = vic_cp(sw(cp));
									}
								}
								else {
									reach_4 = -1;
								}

								if (check_4 == 0) {
									//*************************
									for (depth5 = 0; depth5 < 12; depth5++) { // 5手先
									mini5:

										check_eval(1);
										check_eval(2);

										if (cp == cp3) {
											if (not[eval_sort[depth5]] >= 0 || (reach_4 != -1 && reach_4 != eval_sort[depth5])) {
												depth5++;
												if (depth5 > 11) {
													break;
												}
												goto mini5;
											}
										}
										else {
											if (yes[eval_sort[depth5]] >= 0 || (reach_4 != -1 && reach_4 != eval_sort[depth5])) {
												depth5++;
												if (depth5 > 11) {
													break;
												}
												goto mini5;
											}
										}

										p = top(cp, eval_sort[depth5], &check_i5, &check_j5);
										if (disp3 == 1) {
											display();  ///////////////
										}
										if (p == 1) {
											depth5++;
											if (depth5 > 11) {
												break;
											}
											goto mini5;
										}

										check_4 = check(check_i5, check_j5, cp);

										if (check_4 == 0) {
										//-------------------------
										check_eval(1);
										check_eval(2);

										ie = 3;
										for (m = je; m >= 3; m--) {
											if (ban[m][0] == 0 && ban[m][1] == 0 && ban[m][2] == 0 && ban[m][3] == 0 && ban[m][4] == 0 && ban[m][5] == 0 && ban[m][6] == 0 && ban[m][7] == 0 && ban[m][8] == 0 && ban[m][9] == 0 && ban[m][10] == 0 && ban[m][11] == 0) {
												ie = m;
												break;
											}
										}

										memset(ban_1, 0, sizeof(ban_1));
										memset(ban_2, 0, sizeof(ban_2));

										//空中にあるtrapreach＆空中Wreachを調べる
										check_air_wreach_1_flag = check_air_wreach(1);
										check_air_wreach_2_flag = check_air_wreach(2);

										//trapreachを調べる
										trapreach_flag = trapreach(cp);

										eval_sum = eval1(check_i4, check_j4) + eval2(check_i4, check_j4); //現在の局面を評価

										if (cp_flag1 == 1 && cp_flag2 == 1) { //vic_cpが先手後手両方1の場合、自分の評価値は取り消す
												eval_sum = eval_sum - p19;
										}

										ban[check_i5][check_j5] = 0; //配置戻し
										count++; //eval関数を呼び出した回数をカウントする

										if (eval_sum > eval_min4) { // 枝刈り
											eval_max5 = INFINITY;
											break;
										}

										if (eval_sum > eval_max5) {
											eval_max5 = eval_sum;
										}
										//-------------------------
										}
										else {
											ban[check_i5][check_j5] = 0; //配置戻し
										}
									}
									//*************************

									ban[check_i4][check_j4] = 0; //配置戻し

									if (eval_max5 == -INFINITY) {
										eval_max5 = INFINITY;
									}

									eval_sum = eval_max5;

									if (eval_sum < eval_max3) { // 枝刈り
										eval_min4 = -INFINITY;
										break;
									}

									if (eval_sum < eval_min4) {
										eval_min4 = eval_sum;
									}
								}
								else {
									ban[check_i4][check_j4] = 0; //配置戻し
								}
							}
							//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
							ban[check_i3][check_j3] = 0; //配置戻し

							if (eval_min4 == INFINITY) {
								eval_min4 = -INFINITY;
							}

							eval_sum = eval_min4;

							if (eval_sum == 0) {
			//					display();//////////////////////////////////////////////////////////////////////////////////////-----------------------------
							}

							if (eval_sum > eval_min2) { // 枝刈り
								eval_max3 = INFINITY;
								break;
							}

							if (eval_sum > eval_max3) {
								eval_max3 = eval_sum;
							}
						}
						else {
							ban[check_i3][check_j3] = 0; //配置戻し
						}
					}

					//xxxxxxxxxxxxxxxxxxxxxxxxx
					ban[check_i2][check_j2] = 0; //配置戻し

					if (eval_max3 == -INFINITY) {
						eval_max3 = INFINITY;
					}

					eval_sum = eval_max3;

					if (eval_sum < eval_max1) { // 枝刈り
						eval_min2 = -INFINITY;
						break;
					}

					if (eval_sum < eval_min2) {
						eval_min2 = eval_sum;
					}
				}
				else {
					ban[check_i2][check_j2] = 0; //配置戻し
				}
				//if (depth1 == 5 && eval_min2 == INFINITY) {
				//	printf("");
				//}
			}
			//ooooooooooooooooooooooooo
			ban[check_i][check_j] = 0; //配置戻し

			if (eval_min2 == INFINITY) {
				eval_min2 = -INFINITY;
			}

			eval_sum = eval_min2;
			node_max[12 * 1 - 12 + eval_sort[depth1]] = eval_min2;
			if (eval_sum > eval_max1) {
				eval_max1 = eval_sum;
				cpu = eval_sort[depth1];
			}
			if (eval_sort[depth1] <= 9) {
				printf("node_max[%d] = %5.1f\n", eval_sort[depth1], node_max[eval_sort[depth1]]);
			}
			else if (eval_sort[depth1] == 10) {
				printf("node_max[A] = %5.1f\n", node_max[eval_sort[depth1]]);
			}
			else {
				printf("node_max[B] = %5.1f\n", node_max[eval_sort[depth1]]);
			}

	}

	ct = count;
	//printf("評価回数: %d\n", count);
	return 0;
}

//ダブルリーチしていないか調べる
int check_wreach_mini(int cp) {

	int i, c = 0, k = 0, p = 0, y = -3, x = 0, flag = 0, flag2 = 0, flag3 = 0, flag4 = 0, flg = 0, flg2 = 0, flg3 = 0, flg4 = 0, check;
	int check_i2, check_j2;
	int check_j3, check_i3, check_i4, check_j4;
	int x_s2 = -3, x_e2 = 3;
	int not_check_wreach[12], yes_check_wreach[12];

	check_eval(sw(cp));
	check_eval(cp);
	memcpy(not_check_wreach, not, sizeof(not));
	memcpy(yes_check_wreach, yes, sizeof(yes));

	for (i = 0; i < 12; i++) {
	g1:

		if (yes[i] >= 0 && cp == cp3) {
			flg = 1;
			goto a;
		}
		else {
			flg = 0;
		}
		if (not[i] >= 0 && cp != cp3) {
			flg = 1;
			//      printf("ああ\n");
			goto a;
		}
		else {
			flg = 0;
		}
	a:
		if (cp == cp3) {
			while (1) {
				if (not2[i] >= 0 || not[i] >= 0) {
					i++;
					if (i > 11) {
						break;
					}
					goto g1;
				}
				else {
					break;
				}
			}
			if (i > 11) {
				break;
			}
		}
		if (cp != cp3) {
			while (1) {
				if (yes2[i] >= 0 || yes[i] >= 0) {
					i++;
					if (i > 11) {
						break;
					}
					goto g1;
				}
				else {
					break;
				}
			}
			if (i > 11) {
				break;
			}
		}

		p = top(cp, i, &check_i2, &check_j2);
		//	display();
		if (p == 1) {
			i++;
			if (i == 12) {
				return -1;
			}
			goto g1;
		}

		check_eval(sw(cp));
		check_eval(cp);

		if (flg == 1) {
			if (cp == cp3 && yes_check_wreach[i] >= 0) {
				flag3++;
			}
			else if (cp != cp3 && not_check_wreach[i] >= 0) {
				flag3++;
			}
		}

		if (yes_check_wreach[i] >= 0 && cp == cp3) {
			p = top(sw(cp), i, &check_i3, &check_j3);
			//	display();
			if (p != 1) {
				p = top(cp, i, &check_i4, &check_j4);
				//	display();
				if (p != 1) {
					flg2 = check_LR(check_i4, check_j4, cp);

					if (flg2 == 0) {
						flg2 = check_UD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_LURD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_RULD(check_i4, check_j4, cp);
					}
					ban[check_i4][check_j4] = 0;
				}

				if (flg2 == 1) {
					cpu = i;
					ban[check_i3][check_j3] = 0;
					ban[check_i2][check_j2] = 0;
					//      printf("ああ\n");
					return 1;
				}
				ban[check_i3][check_j3] = 0;
			}
		}
		if (not_check_wreach[i] >= 0 && cp != cp3) {
			//            printf("いい");
			p = top(sw(cp), i, &check_i3, &check_j3);
			if (p != 1) {
				p = top(cp, i, &check_i4, &check_j4);
				if (p != 1) {
					flg2 = check_LR(check_i4, check_j4, cp);

					if (flg2 == 0) {
						flg2 = check_UD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_LURD(check_i4, check_j4, cp);
					}
					if (flg2 == 0) {
						flg2 = check_RULD(check_i4, check_j4, cp);
					}
					ban[check_i4][check_j4] = 0;
				}

				if (flg2 == 1) {
					cpu = i;
					ban[check_i3][check_j3] = 0;
					ban[check_i2][check_j2] = 0;
					// 	     printf("いい\n");
					return 1;
				}
				ban[check_i3][check_j3] = 0;
			}
		}

		if (cp == cp3) {
			check_eval(sw(cp3));
		}
		else {
			check_eval(cp3);
		}

		if (i - 3 < 0) {
			x_s2 = 0 - i;
		}
		if (i + 3 > 11) {
			x_e2 = 11 - i;
		}

		//***駒を置いた左右3マスを調べる***
		for (k = x_s2; k <= x_e2; k++) {
		check_wreach2:

			p = top(cp, i + k, &check_i3, &check_j3);
			//		display();
			if (p == 1) {
				k++;
				if (k > x_e2) {
					break;
				}
				goto check_wreach2;
			}

			//***駒を置いた左右3マスを調べる***
			flag = check_LR(check_i2, check_j2, cp);
			if (flag == 1) {
				flag2 = 1;
			}

			//***駒を置いた場所から上下3マスを調べる***
			if (flag == 0) {
				flag = check_UD(check_i2, check_j2, cp);
				if (flag == 1 && flg == 0) {
					flag2 = 1;
					if (cp == cp3) {
						if (yes[i] >= 0) {
							cpu = i;
							ban[check_i3][check_j3] = 0;
							ban[check_i2][check_j2] = 0;
							return 1;
						}
					}
					else {
						if (not[i] >= 0) {
							cpu = i;
							ban[check_i3][check_j3] = 0;
							ban[check_i2][check_j2] = 0;
							return 1;
						}
					}
				}
			}

			//***駒を置いた場所から左上～右下3マスを調べる***
			if (flag == 0) {
				flag = check_LURD(check_i2, check_j2, cp);
				if (flag == 1) {
					flag2 = 1;
				}
			}

			//***駒を置いた場所から右上～左下3マスを調べる***
			if (flag == 0) {
				flag = check_RULD(check_i2, check_j2, cp);
				if (flag == 1) {
					flag2 = 1;
				}
			}

			ban[check_i3][check_j3] = 0;

			if (cp == cp3) {
				if (flag == 1 && yes_check_wreach[i] >= 0 && flg == 0) {
					cpu = check_j2;
					ban[check_i2][check_j2] = 0;
					return 1;
				}
			}
			else {
				if (flag == 1 && not_check_wreach[i] >= 0 && flg == 0) {
					cpu = check_j2;
					ban[check_i2][check_j2] = 0;
					return 1;
				}
			}

			if (flag2 == 1) {
				flag3++;
			}
			if (flag3 > 1) {
				ban[check_i2][check_j2] = 0;
				if (cp == cp3) {
					if (not2[check_j2] == -1) {
						cpu = check_j2;
						return 1;
					}
				}
				if (cp != cp3) {
					if (yes2[check_j2] == -1) {
						cpu = check_j2;
						return 1;
					}
				}
			}
			flag2 = 0;
		}	    //for文kの終わり

		flag3 = 0;
		flag2 = 0;
		flg = 0;
		x_s2 = -3;
		x_e2 = 3;
		ban[check_i2][check_j2] = 0;
	}  //for文iの終わり
	return -1;
}