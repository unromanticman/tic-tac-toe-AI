#include <iostream>
#include <algorithm>
using namespace std;

/**
 * 三子棋 AI ( tic tac toe AI )
 * Date:2017/10/09
 * Author: UM
 */

// function minimax(node, depth, maximizingPlayer)
//      if depth = 0 or node is a terminal node
//          return the heuristic value of node

//      if maximizingPlayer
//          bestValue := −∞
//          for each child of node
//              v := minimax(child, depth − 1, FALSE)
//              bestValue := max(bestValue, v)
//          return bestValue

//      else    (* minimizing player *)
//          bestValue := +∞
//          for each child of node
//              v := minimax(child, depth − 1, TRUE)
//              bestValue := min(bestValue, v)
//          return bestValue
// (* Initial call for maximizing player *)
// minimax(origin, depth, TRUE)



//AI與PLAYER的棋子
#define AI 	    2
#define PLAYER  1

//移動的步與權重
class Move{
public:
	int x,y,value;
	Move(){}
};

//棋盤
class Board{

private:
	//棋盤二維陣列
	int ** board;
public:
	Board(){
		//初始化棋盤二維陣列為0
		board = new int*[3];
		for(int i=0;i<3;i++){
			board[i] = new int[3];
			for(int j=0;j<3;j++){
				board[i][j] = 0;
			}
		}
	}

	//取得棋盤的某一個棋子
	int getChess(int x,int y){
		return board[x][y];
	}

	//畫出棋盤
	void showBoard(){
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				cout<< board[i][j]<<' ';
			}
			cout<<'\n';
		}
	}
	//設定棋盤上的棋子
	bool setChess(int chess,int x,int y){
		//如果為0代表還沒下過，將棋子下於此處，並且回傳true
		if(board[x][y] == 0){
			board[x][y] = chess;	
			return true;
		}
		//如果被下過則回傳false
		return false;
	}

	//取得棋盤陣列
	int ** getBoardArr(){
		return board;
	}

	//複製新棋盤，並取代本來棋盤，此為避免C++ 參考關係才寫的函式
	void copyBoard(int ** newboard){
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				board[i][j] = newboard[i][j];
			}
		}
	}

	//判斷是否棋盤都被下滿無法落子 被下滿則回傳true 反之
	bool isNoMove(){
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				if(board[i][j] == 0){
					return false;
				}
			}
		}
		return true;
	}

	//檢查是否遊戲結束
	bool isGameOver(){

		//橫
		for (int i = 0; i < 3; i++){
			if(board[i][0]==board[i][1] && board[i][1] == board[i][2] ){
				//如果達成橫3 並且 內容不是 0 (0代表沒有下過的點)
				if((board[i][0]) != 0 ){
					return true;	
				}
			}
		}

		//直
		for (int i = 0; i < 3; i++){
			if(board[0][i]==board[1][i] && board[1][i] == board[2][i]){
				//如果達成直3 並且 內容不是 0 (0代表沒有下過的點)
				if((board[0][i]) != 0 ){
					return true;	
				}
			}
		}

		//斜邊右
		if(board[0][0]==board[1][1] && board[1][1] == board[2][2]){
			//如果達右斜 並且 內容不是 0 (0代表沒有下過的點)
			if((board[0][0]) != 0 ){
				return true;	
			}
		}

		//斜邊左
		if(board[2][0]==board[1][1] && board[1][1] == board[0][2]){
			//如果達左斜 並且 內容不是 0 (0代表沒有下過的點)
			if((board[2][0]) != 0 ){
					return true;	
			}
		}

		//如果還沒分出勝負
		return false;
	}

	//計算盤末的權重
	int evaluate(){

		//橫
		for (int i = 0; i < 3; i++){
			if(board[i][0]==board[i][1] && board[i][1] == board[i][2]){
				if(board[i][0] == AI){
					//如果是讓AI贏則拿10分
					return 10;
				}else if(board[i][0] == PLAYER){
					//如果是讓玩家贏則拿-10分
					return -10;
				}
			}
		}

		//直
		for (int i = 0; i < 3; i++){
			if(board[0][i]==board[1][i] && board[1][i] == board[2][i]){
				if(board[0][i] == AI){
					//如果是讓AI贏則拿10分
					return 10;
				}else if(board[0][i] == PLAYER){
					//如果是讓玩家贏則拿-10分
					return -10;
				}
			}
		}

		//斜邊右
		if(board[0][0]==board[1][1] && board[1][1] == board[2][2]){
				if(board[1][1] == AI){
					//如果是讓AI贏則拿10分
					return 10;
				}else if(board[1][1] == PLAYER){
					//如果是讓玩家贏則拿-10分
					return -10;
				}
		}

		//斜邊左
		if(board[2][0]==board[1][1] && board[1][1] == board[0][2]){
				if(board[1][1] == AI){
					//如果是讓AI贏則拿10分
					return 10;
				}else if(board[1][1] == PLAYER){
					//如果是讓玩家贏則拿-10分
					return -10;
				}
		}

		//如果平局則回傳0不影響分數
		return 0;
	}

	
};

//遍歷棋盤搭配最大最小演算法計算每棋子導致的最後權重
int minimax(Board board,bool maximizingPlayer){
	//如果棋盤分出勝負
	if(board.isGameOver()){
		//回傳盤末權重
		return board.evaluate();
	}

	//如果棋盤無法在下棋子 表示平局
	if(board.isNoMove()){
		return 0;
	}


	if(maximizingPlayer){
		//找出我方(AI)最大的值
		//建立一個負無窮大變數
		int bestValue = -1e9;
		//遍利每個棋盤找出可以落子處，並找出最大值
		for(int i=0;i<3;i++){
			for (int j=0; j < 3; j++){
				//複製棋盤 避免C++參考影響後續盤面問題
				Board temp;
				temp.copyBoard(board.getBoardArr());
				//找出可以落子處
				if(temp.setChess(AI,i,j)){
					//在此層找出最大值
					int val = minimax(temp,!maximizingPlayer);
					bestValue = max(val,bestValue);
				}
			}
		}
		return bestValue;
	}else{
		//找出敵方(PLAYER)最小的值
		//建立一個正無窮大變數
		int bestValue = 1e9;
		//遍利每個棋盤找出可以落子處，並找出最小值
		for(int i=0;i<3;i++){
			for (int j=0; j < 3; j++){
				//複製棋盤 避免C++參考影響後續盤面問題
				Board temp;
				temp.copyBoard(board.getBoardArr());
				//找出可以落子處
				if(temp.setChess(PLAYER,i,j)){
					//在此層找出最小值
					int val = minimax(temp,!maximizingPlayer);
					bestValue = min(val,bestValue);
				}
			}
		}
		return bestValue;
	}

}

//以權重衡量最佳的落子處
Move bestMove(Board board){

	//在此層找出最大值 (第0層)
	Move move;
	move.value = -1e9;
	move.x = -1;
	move.y = -1;


	//遍歷每個可以落子的點
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			//複製棋盤 避免C++參考影響後續盤面問題
			Board temp;
			temp.copyBoard(board.getBoardArr());
			if(temp.setChess(AI,i,j)){
				//找出此層最大值
				int val = minimax(temp,false);
				if(move.value < val){
					move.x = i;
					move.y = j;
					move.value = val;
				}
			}
		}
	}

	//回傳最好的落子點
	return move;
}

int main(){
	//建立一個盤面
	Board b;
	//玩家下棋的座標
	int x,y;
	//回合數
	int count = 0 ;

	//無窮迴圈直到一方勝出或平均
	while(1){
		cout<<"===\t第"<<++count<<"回合\t==="<<endl;
		cout<<"請輸入 x y 座標"<<endl;
		cin>>x>>y;
		//玩家下棋
		b.setChess(PLAYER,x,y);	
		//顯示玩家下過的棋盤
		cout<<"PLAYER:"<<endl;
		b.showBoard();
		//如果此次下完終局表示玩家勝出
		if(b.isGameOver()){
			cout<<"PLAYER WIN"<<endl;
			break;
		}
		//AI下棋
		Move move = bestMove(b);
		b.setChess(AI,move.x,move.y);
		cout<<"AI:"<<endl;
		//顯示AI下完的棋盤 與 x y 座標 權重
		b.showBoard();
		cout<<"AI權重計算結果"<<"\nx:"<<move.x<<"\t"<<"y:"<<move.y<<"\tvalue:"<<move.value<<endl;
		//如果此次下完終局表示AI勝出
		if(b.isGameOver()){
			cout<<"AI WIN"<<endl;
			break;
		}
		//如果沒有辦法再落子則離開迴圈 平局
		if(b.isNoMove()){
			cout<<"TIE"<<endl;
			break;
		}
	}

}


// 感謝以下文章 讓我了解minimax演算法:
// http://programmermagazine.github.io/201407/htm/focus3.html
// https://gist.github.com/MatthewSteel/3158579
// https://github.com/jieaozhu/Machine_Learning/blob/master/minimax_alpha_beta_pruning/basicplayer.py#L48
// https://www.zhihu.com/question/27221568/answer/127599152
// http://blog.csdn.net/joshualiunsw/article/details/52131507
// http://blog.csdn.net/tianzhijiaozi19/article/details/52416025
// https://en.wikipedia.org/wiki/Minimax
// http://neverstopbuilding.com/minimax
// http://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-3-tic-tac-toe-ai-finding-optimal-move/
// https://medium.freecodecamp.org/how-to-make-your-tic-tac-toe-game-unbeatable-by-using-the-minimax-algorithm-9d690bad4b37