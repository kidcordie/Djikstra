#include "Djik.cpp"
#include <iostream>

int main(){
		Adj_List Ad;
		Ad.randomAdj(100, 5);
		Ad.djikstra(Ad.FindStart());
		//Ad.print();
}
