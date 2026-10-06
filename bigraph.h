#pragma once
#include"utility.h"


class BiGraph {
public:
	//Biparit Grapha
	int n1, n2, m, kw, maxAlpha, maxBeta, maxUDeg, maxVDeg, maxK;
	vector<int>uDeg, vDeg;
	vector<vector<int>> uNeighbor, vNeighbor;
	vector<vector<int>> keyWords;

	//Index
	vector<vector<int>> uAlphaOffset, uBetaOffset, vAlphaOffset, vBetaOffset;
	vector<vector<int>> IL;
	//vector <vector<vector<pair<int, bool>>>> alphaOrders, betaOrders;
	//vector<vector<int>> uAlphaOrder, vAlphaOrder;
	//vector<vector<int>> uBetaOrder, vBetaOrder;
	
public:
	//base
	BiGraph() {}
	BiGraph(string str) {
		n1 = 0;
		n2 = 0;
		m = 0;
		loadGraph(str);
		cout << "n1: " << n1 << " n2: " << n2 << endl;
		cout << "m: " << m << endl;
	}

	void addEdge(int u, int v) {
		m++;
		uNeighbor[u].push_back(v);
		vNeighbor[v].push_back(u);
		uDeg[u]++;
		vDeg[v]++;
		if (maxUDeg < uDeg[u])
			maxUDeg = uDeg[u];
		if (maxVDeg < vDeg[v])
			maxVDeg = vDeg[v];
	}

	void deleteEdge(int u, int v) {
		m--;
		auto it = remove(uNeighbor[u].begin(), uNeighbor[u].end(), v);
		uNeighbor[u].erase(it, uNeighbor[u].end());
		uDeg[u]--;
		it = remove(vNeighbor[v].begin(), vNeighbor[v].end(), u);
		vNeighbor[v].erase(it, vNeighbor[v].end());
		vDeg[v]--;
	}

	void loadGraph(string str) {
		string gstr = str + "/graph.txt";
		string estr = str + "/edge.txt";
		string wstr = str + "/word.txt";
		FILE* g = fopen(gstr.c_str(), "r");
		fscanf(g, "%d %d %d %d", &n1, &n2, &m, &kw);
		fclose(g);
		init(n1, n2);
		FILE* e = fopen(estr.c_str(), "r");
		int u, v, w;
		int mm = 0;
		while ((fscanf(e, "%d %d %d", &u, &v, &w)) != EOF) {
			/*mm++;
			if(mm>= 1562745)*/
				addEdge(u, v);
			/*if (m == 6298981)
				break;*/
		}
		fclose(e);
		FILE* word = fopen(wstr.c_str(), "r");
		int wd;
		for (int i = 1; i <= n2; i++) {
			fscanf(word, "%d %d", &v, &wd);
			for (int j = 0; j < wd; j++) {
				int ww;
				fscanf(word, "%d", &ww);
				keyWords[v].push_back(ww);
			}
		}
		fclose(word);
	}

	void init(int n1_, int n2_) {
		n1 = n1_;
		n2 = n2_;
		m = 0;
		uDeg.resize(n1 + 1, 0);
		vDeg.resize(n2 + 1, 0);
		uNeighbor.resize(n1 + 1);
		vNeighbor.resize(n2 + 1);
		keyWords.resize(n2 + 1);
		maxUDeg = 0;
		maxVDeg = 0;
	}

public:
	//Algorithms
	void Global(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
	void Local(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
	void Local2(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
	bool Verification(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
	bool Verification2(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
	bool checkKeywords(int v, vector<int> &QK);
	bool checkKeywords2(int v, vector<int> &QK);
	void DeleteCand(int alpha, int beta, vector<int>& uResults, vector<int>& vResults, 
		            vector<int> &degU, vector<int> &degV, queue<int> &qu, queue<int> &qv);
	void DeleteCand2(int alpha, int beta, vector<int>& uResults, vector<int>& vResults,
		vector<int>& degU, vector<int>& degV, queue<int>& qu, queue<int>& qv);
	void DeleteCand3(int alpha, int beta, vector<int>& uVis, vector<int>& vVis,
		vector<int>& degU, vector<int>& degV, queue<int>& qu, queue<int>& qv);
	void DeleteCandForCore(int alpha, int beta, vector<int>& uResults, vector<int>& vResults, vector<int>& uVis, vector<int>& vVis,
		vector<int>& degU, vector<int>& degV, queue<int>& qu, queue<int>& qv);

	//Index
	void BulidIndex();
	//alpha、beta分解
	int coreDecompose();
	int alphaDecompose(int alpha, vector<int>& leftDeg, vector<int>& rightDeg, vector<bool>& uDelete,
		vector<bool>& vDelete, vector<vector<pair<int, bool>>>& order);
	int betaDecompose(int beta, vector<int>& leftDeg, vector<int>& rightDeg, vector<bool>& uDelete,
		vector<bool>& vDelete, vector<vector<pair<int, bool>>>& order);

	//IndexBasedSearch
	void GobalBasedIndex(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
	void LocalBasedIndex(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
	void LocalBasedSearch2(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults);
};

void BiGraph::Global(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults) {

	/*vector<int> tempQK(kw + 1, 0);
	for (int qk : QK)
		tempQK[qk] = 1;
	QK = tempQK;*/

	vector<int> uVis(n1 + 1, true);
	vector<int> vVis(n2 + 1, true);
	vector<int> degU = uDeg;
	vector<int> degV = vDeg;
	queue<int> qu, qv;

	for (int u = 1; u <= n1; u++) 
		if (degU[u] < alpha) {
			qu.push(u);
			uVis[u] = false;
		}
	for (int v = 1; v <= n2; v++)
		if (degV[v] < beta || !checkKeywords(v, QK)) {
			qv.push(v);
			vVis[v] = false;
		}

	DeleteCand3(alpha, beta, uVis, vVis, degU, degV, qu, qv);

	bool qOK = isU ? uVis[q]: vVis[q];

	if (qOK) {
		queue<pair<int, bool>> bfs;
		bfs.push(make_pair(q, isU));
		if (isU) uResults[q] = 1; else vResults[q] = 1;
		while (!bfs.empty()) {
			pair<int, bool> cur = bfs.front(); 
			bfs.pop();
			if (cur.second) {
				for (int v : uNeighbor[cur.first]) 
					if (vVis[v]&&!vResults[v]) {
						vResults[v] = 1;
						bfs.push(make_pair(v, false));
					}
			}
			else {
				for (int u : vNeighbor[cur.first]) {
					if (uVis[u]&&!uResults[u]) {
						uResults[u] = 1;
						bfs.push(make_pair(u, true));
					}
				}
			}
		}
	}
}

void BiGraph::Local(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults) {
	vector<int> degU = uDeg, degV = vDeg;
	queue<pair<int, bool>> bfs;
	queue<int> qu, qv;

	if (isU && degU[q] >= alpha) {
		bfs.push(make_pair(q, isU));
		uResults[q] = 1;
	}
	else if (!isU && degV[q] >= beta && checkKeywords(q,QK)) {
		bfs.push(make_pair(q, isU));
		vResults[q] = 1;
	}

	while (!bfs.empty()) {
		pair<int, bool> cur = bfs.front();
		bfs.pop();
		if (cur.second) {
			if (degU[cur.first] >= alpha) {
				for (int v : uNeighbor[cur.first]) {
					if (degV[v] >= beta && checkKeywords(v, QK) && vResults[v] == 0) {
						bfs.push(make_pair(v, false));
						vResults[v] = 1;
					}
					else if (vResults[v] == 0) {
						qv.push(v);
						vResults[v] = -1;
					}
				}
			}
			else {
				qu.push(cur.first);
				uResults[cur.first] = -1;
			}
		}
		else {
			if (degV[cur.first] >= beta) {
				for (int u : vNeighbor[cur.first]) {
					if (degU[u] >= alpha && uResults[u] == 0) {
						bfs.push(make_pair(u, true));
						uResults[u] = 1;
					}
					else if ( uResults[u] == 0) {
						qu.push(u);
						uResults[u] = -1;
					}
				}
			}
			else {
				qv.push(cur.first);
				vResults[cur.first] = -1;
			}
		}
	}

	DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);

	bool isQ = true;
	if (isU && uResults[q]==-1) 
		isQ = false;
	else if (!isU && vResults[q] == -1) 
		isQ = false;
	if (!isQ) {
		for (int i = 0; i <= n1; i++) uResults[i] = 0;
		for (int i = 0; i <= n2; i++) vResults[i] = 0;
	}
}

bool BiGraph::checkKeywords(int v, vector<int> &QK) {
	unordered_set<int> qk;
	for (auto k : QK)
		qk.insert(k);
	if (qk.empty()) 
		return true;
	for (int k : keyWords[v]) 
		if (qk.count(k)) 
			return true;
	return false;
}

bool BiGraph::Verification(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults) {
	
	int count1 = 0, count2 = 0;
	for (auto i : uResults) if (i==1) count1++;
	for (auto i : vResults) if (i==1) count2++;

	bool qOk = isU ? uResults[q] : vResults[q];
	if (count1 == 0 && count2 == 0 && !qOk)
		return true;

	vector<bool> visU(n1 + 1, false), visV(n2 + 1, false);
	queue<pair<int, bool>> bfs;
	bfs.push(make_pair(q, isU));
	if (isU) 
		visU[q] = true; 
	else 
		visV[q] = true;

	while (!bfs.empty()) {
		pair<int, bool> cur = bfs.front(); 
		bfs.pop();
		if (cur.second) {
			int deg = 0;
			for (int v : uNeighbor[cur.first]) {
				if (vResults[v]==1) {
					deg++;
					if (!visV[v]) {
						visV[v] = true;
						bfs.push(make_pair(v, false));
					}
				}
			}
			if (deg < alpha)
				return false;
		}
		else {
			int deg = 0;
			for (int u : vNeighbor[cur.first]) {
				if (uResults[u]==1) {
					deg++;
					if (!visU[u]) {
						visU[u] = true;
						bfs.push(make_pair(u, true));
					}
				}
			}
			if (deg < beta)
				return false;
		}
	}
	return true;
}

bool BiGraph::Verification2(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults) {
	vector<int> nor_uResults(n1 + 1, 0);
	vector<int> nor_vResults(n2 + 1, 0);
	Global(alpha, beta, q, isU, QK, nor_uResults, nor_vResults);
	for (int i = 1; i <= n1; i++)
		if (nor_uResults[i] != uResults[i]) {
			if (nor_uResults[i] == 0 && uResults[i] == -1)
				;
			else
				return false;
		}
	for (int i = 1; i <= n2; i++)
		if (nor_vResults[i] != vResults[i]) {
			if (nor_vResults[i] == 0 && vResults[i] == -1)
				;
			else
				return false;
		}
	return true;
}

void BiGraph::Local2(int alpha, int beta, int q, bool isU, vector<int> QK, vector<int>& uResults, vector<int>& vResults) {
	
	vector<int> tempQK(kw + 1, 0);
	for (int qk : QK)
		tempQK[qk] = 1;
	QK = tempQK;

	vector<int> degU = uDeg, degV = vDeg;
	queue<pair<int, bool>> bfs;
	queue<int> qu, qv;

	if (isU && degU[q] >= alpha) {
		bfs.push(make_pair(q, isU));
		uResults[q] = 1;
	}
	else if (!isU && degV[q] >= beta && checkKeywords2(q, QK)) {
		bfs.push(make_pair(q, isU));
		vResults[q] = 1;
	}

	while (!bfs.empty()) {
		pair<int, bool> cur = bfs.front();
		bfs.pop();
		if (cur.second) {
			if (uResults[cur.first] == -1)
				continue;
			if (degU[cur.first] >= alpha) {
				for (int v : uNeighbor[cur.first]) {
					if (degV[v] >= beta && checkKeywords2(v, QK) && vResults[v] == 0) {
						bfs.push(make_pair(v, false));
						vResults[v] = 1;
					}
					else if (vResults[v] == 0) {
						qv.push(v);
						vResults[v] = -1;
						DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
					}
				}
			}
			else {
				qu.push(cur.first);
				uResults[cur.first] = -1;
				DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
			}
		}
		else {
			if (vResults[cur.first] == -1)
				continue;
			if (degV[cur.first] >= beta) {
				for (int u : vNeighbor[cur.first]) {
					if (degU[u] >= alpha && uResults[u] == 0) {
						bfs.push(make_pair(u, true));
						uResults[u] = 1;
					}
					else if (uResults[u] == 0) {
						qu.push(u);
						uResults[u] = -1;
						DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
					}
				}
			}
			else {
				qv.push(cur.first);
				vResults[cur.first] = -1;
				DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
			}
		}
	}
	bool isQ = true;
	if (isU && uResults[q] == -1)
		isQ = false;
	else if (!isU && vResults[q] == -1)
		isQ = false;
	if (!isQ) {
		for (int i = 0; i <= n1; i++) uResults[i] = 0;
		for (int i = 0; i <= n2; i++) vResults[i] = 0;
	}
}

void BiGraph::DeleteCand(int alpha, int beta, vector<int>& uResults, vector<int>& vResults,
	vector<int>& degU, vector<int>& degV, queue<int>& qu, queue<int>& qv) {
	
	while (!qu.empty() || !qv.empty()) {
		while (!qu.empty()) {
			int u = qu.front();
			qu.pop();
			for (int v : uNeighbor[u]) {
				--degV[v];
				if (vResults[v] == 1) {
					if (degV[v] < beta) {
						qv.push(v);
						vResults[v] = -1;
					}
				}
			}
		}
		while (!qv.empty()) {
			int v = qv.front(); qv.pop();
			for (int u : vNeighbor[v]) {
				--degU[u];
				if (uResults[u] == 1) {
					if (degU[u] < alpha) {
						qu.push(u);
						uResults[u] = -1;
					}
				}
			}
		}
	}
}


void BiGraph::BulidIndex() {
	
	vector<bool>uDel(n1 + 1, false), vDel(n2 + 1, false);
	vector<int> tempDegU = uDeg;
	vector<int> tempDegV = vDeg;
	vector<vector<pair<int, bool>>> order;

	maxK = coreDecompose();
	cout << "max kcore:" << maxK << endl;

	uAlphaOffset.resize(n1 + 1);
	uBetaOffset.resize(n1 + 1);
	vAlphaOffset.resize(n2 + 1);
	vBetaOffset.resize(n2 + 1);

	//alphaOrders.resize(1);
	//betaOrders.resize(1);

	for (int alpha = 1; alpha <= maxK; alpha++) {
		alphaDecompose(alpha, uDeg, vDeg, uDel, vDel, order);
		int o = 0;
		for (int i = 0; i < order.size(); i++) {
			for (auto& j : order[i]) {
				if (j.second) {
					uAlphaOffset[j.first].push_back(i + 1);
					//uAlphaOrder[j.first].push_back(o);
				}
				else {
					vAlphaOffset[j.first].push_back(i + 1);
				}
				o++;
			}
		}
		//alphaOrders.push_back(order);
		order.clear();
	}
	for (int beta = 1; beta <= maxK; beta++) {
		betaDecompose(beta, uDeg, vDeg, uDel, vDel, order);
		for (int i = 0; i < order.size(); i++) {
			for (auto& j : order[i]) {
				if (j.second) {
					uBetaOffset[j.first].push_back(i + 1);
				}
				else {
					vBetaOffset[j.first].push_back(i + 1);
				}
			}
		}
		//betaOrders.push_back(order);
		order.clear();
	}
	uDeg = tempDegU;
	vDeg = tempDegV;

	IL.resize(kw + 1);
	for (int i = 1; i <= n2; i++)
		for (auto k : keyWords[i])
			IL[k].push_back(i);

}

int BiGraph::coreDecompose() {
	vector<int> leftQ;//左侧删除队列
	vector<int> rightQ;//右侧删除队列

	int num1 = n1 + 1;//左侧节点数量
	vector<int> leftR(num1);//用于存储当前迭代中尚未被删除的顶点
	for (int i = 0; i < leftR.size(); i++)
		leftR[i] = i;

	int leftRTnum = 0;//用于记录在当前迭代中尚未被删除的顶点数量。
	vector<int> leftRT(num1);//用于临时存储在当前迭代中尚未被删除的顶点

	int num2 = n2 + 1;//
	vector<int> rightR(num2);//用于存储当前迭代中尚未被删除的顶点
	for (int i = 0; i < rightR.size(); i++)
		rightR[i] = i;

	int rightRTnum = 0;//用于记录在当前迭代中尚未被删除的顶点数量。
	vector<int> rightRT(num2);//用于临时存储在当前迭代中尚未被删除的顶点

	vector<bool>leftDel(num1, false), rightDel(num2, false);
	vector<int>leftDeg = uDeg, rightDeg = vDeg;

	int kc = 1;
	for (kc = 1; kc <= maxVDeg + 1; kc++) {
		bool stop = true;
		leftRTnum = 0;
		for (int i = 0; i < num1; i++) {
			int u = leftR[i];
			if (!leftDel[u]) {
				stop = false;
				leftRT[leftRTnum] = u;
				leftRTnum++;
				if (leftDeg[u] < kc) {
					leftQ.push_back(u);
				}
			}
		}
		swap(leftR, leftRT);
		num1 = leftRTnum;
		if (stop)
			break;
		stop = true;
		rightRTnum = 0;
		for (int i = 0; i < num2; i++) {
			int v = rightR[i];
			if (!rightDel[v]) {
				stop = false;
				rightRT[rightRTnum] = v;
				rightRTnum++;
				if (rightDeg[v] < kc) {
					rightQ.push_back(v);
				}
			}
		}
		swap(rightR, rightRT);
		num2 = rightRTnum;
		if (stop)
			break;
		while (!leftQ.empty() || !rightQ.empty()) {

			for (auto j = leftQ.begin(); j != leftQ.end(); j++) {
				int u = *j;
				if (leftDel[u])
					continue;
				for (int k = 0; k < uNeighbor[u].size(); k++) {
					int v = uNeighbor[u][k];
					if (rightDel[v])
						continue;
					rightDeg[v]--;

					if (rightDeg[v] == 0) {
						rightDel[v] = true;
					}
					if (rightDeg[v] < kc) {
						rightQ.push_back(v);
					}
				}
				leftDeg[u] = 0;
				leftDel[u] = true;
			}
			leftQ.clear();

			for (auto j = rightQ.begin(); j != rightQ.end(); j++) {
				int v = *j;
				if (rightDel[v])
					continue;
				for (int k = 0; k < vNeighbor[v].size(); k++) {
					int u = vNeighbor[v][k];
					if (leftDel[u]) continue;

					leftDeg[u]--;
					if (leftDeg[u] == 0) {
						leftDel[u] = true;
					}
					if (leftDeg[u] < kc) {
						leftQ.push_back(u);
					}
				}
				rightDeg[v] = 0;
				rightDel[v] = true;
			}
			rightQ.clear();
		}
	}
	return kc - 2;
}

int BiGraph::alphaDecompose(int alpha, vector<int>& leftDeg, vector<int>& rightDeg, vector<bool>& uDelete,
	vector<bool>& vDelete, vector<vector<pair<int, bool>>>& order) {

	vector<int>leftQ, rightQ;

	for (int i = 1; i < uDelete.size(); i++)
		if (!uDelete[i] && leftDeg[i] < alpha) {
			leftQ.push_back(i);
			uDelete[i] = true;
		}
	while (!leftQ.empty() || !rightQ.empty()) {
		for (int i = 0; i < leftQ.size(); i++) {
			int u = leftQ[i];
			for (int j = 0; j < uNeighbor[u].size(); j++) {
				int v = uNeighbor[u][j];
				if (vDelete[v])
					continue;
				rightDeg[v]--;
				if (rightDeg[v] == 0) {
					rightQ.push_back(v);
					vDelete[v] = true;
				}
			}
		}
		leftQ.clear();
		for (int i = 0; i < rightQ.size(); i++) {
			int v = rightQ[i];
			for (int j = 0; j < vNeighbor[v].size(); j++) {
				int u = vNeighbor[v][j];
				if (uDelete[u])
					continue;
				leftDeg[u]--;
				if (leftDeg[u] < alpha) {
					leftQ.push_back(u);
					uDelete[u] = true;
				}
			}
		}
		rightQ.clear();
	}
	//初始化
	int num = n2;
	int nextNum = 0;
	vector<pair<int, bool>>bfsQ;
	vector<int>remain(num);
	vector<int>nextRemain(num);
	vector<bool>leftDel = uDelete;
	vector<bool>rightDel = vDelete;
	vector<int>degL = leftDeg;
	vector<int>degR = rightDeg;

	for (int i = 0; i < remain.size(); i++)
		remain[i] = i + 1;
	for (int beta = 1; beta <= maxVDeg + 1; beta++) {
		vector<pair<int, bool>> bh;
		nextNum = 0;
		for (int i = 0; i < num; i++) {
			int v = remain[i];
			if (!rightDel[v]) {
				if (degR[v] <= beta) {
					bfsQ.push_back({ v,false });
					rightDel[v] = true;
					bh.push_back({ v,false });
					for (int i = 0; i < bfsQ.size(); i++) {
						int p = bfsQ[i].first;
						if (bfsQ[i].second) {
							for (int j = 0; j < uNeighbor[p].size(); j++)
								if (!rightDel[uNeighbor[p][j]]) {
									degR[uNeighbor[p][j]]--;
									if (degR[uNeighbor[p][j]] == beta) {
										bfsQ.push_back({ uNeighbor[p][j],false });
										rightDel[uNeighbor[p][j]] = true;
										bh.push_back({ uNeighbor[p][j],false });
									}
								}
						}
						else {
							for (int j = 0; j < vNeighbor[p].size(); j++)
								if (!leftDel[vNeighbor[p][j]]) {
									degL[vNeighbor[p][j]]--;
									if (degL[vNeighbor[p][j]] < alpha) {
										bfsQ.push_back({ vNeighbor[p][j],true });
										leftDel[vNeighbor[p][j]] = true;
										bh.push_back({ vNeighbor[p][j],true });
									}
								}
						}
					}
					bfsQ.clear();
				}
				else {
					nextRemain[nextNum] = v;
					nextNum++;
				}
			}
		}
		order.push_back(bh);
		bh.clear();
		swap(remain, nextRemain);
		num = nextNum;
		if (nextNum == 0)
			break;
	}
	return 0;
}

int BiGraph::betaDecompose(int beta, vector<int>& leftDeg, vector<int>& rightDeg, vector<bool>& uDelete,
	vector<bool>& vDelete, vector<vector<pair<int, bool>>>& order) {
	//
	vector<int>leftQ, rightQ;
	//
	for (int i = 1; i < uDelete.size(); i++)
		if (!uDelete[i] && leftDeg[i] <= maxK) {
			leftQ.push_back(i);
			uDelete[i] = true;
		}
	for (int i = 1; i < vDelete.size(); i++)
		if (!vDelete[i] && rightDeg[i] < beta) {
			rightQ.push_back(i);
			vDelete[i] = true;
		}
	while (!leftQ.empty() || !rightQ.empty()) {
		for (int i = 0; i < leftQ.size(); i++) {
			int u = leftQ[i];
			for (int j = 0; j < uNeighbor[u].size(); j++) {
				int v = uNeighbor[u][j];
				if (vDelete[v])
					continue;
				rightDeg[v]--;
				if (rightDeg[v] < beta) {
					rightQ.push_back(v);
					vDelete[v] = true;
				}
			}
		}
		leftQ.clear();
		for (int i = 0; i < rightQ.size(); i++) {
			int v = rightQ[i];
			for (int j = 0; j < vNeighbor[v].size(); j++) {
				int u = vNeighbor[v][j];
				if (uDelete[u])
					continue;
				leftDeg[u]--;
				if (leftDeg[u] <= maxK) {
					leftQ.push_back(u);
					uDelete[u] = true;
				}
			}
		}
		rightQ.clear();
	}
	//初始化
	int num = n1;
	int nextNum = 0;
	vector<pair<int, bool>>bfsQ;
	vector<int>remain(num);
	vector<int>nextRemain(num);
	vector<bool>leftDel = uDelete;
	vector<bool>rightDel = vDelete;
	vector<int>degL = leftDeg;
	vector<int>degR = rightDeg;

	for (int i = 0; i < remain.size(); i++)
		remain[i] = i + 1;
	for (int alpha = maxK + 1; alpha <= maxUDeg + 1; alpha++) {
		vector<pair<int, bool>> bh;
		nextNum = 0;
		for (int i = 0; i < num; i++) {
			int u = remain[i];
			if (!leftDel[u]) {
				if (degL[u] <= alpha) {
					bfsQ.push_back({ u,true });
					leftDel[u] = true;
					bh.push_back({ u,true });
					for (int i = 0; i < bfsQ.size(); i++) {
						int p = bfsQ[i].first;
						if (bfsQ[i].second) {
							for (int j = 0; j < uNeighbor[p].size(); j++)
								if (!rightDel[uNeighbor[p][j]]) {
									degR[uNeighbor[p][j]]--;
									if (degR[uNeighbor[p][j]] < beta) {
										bfsQ.push_back({ uNeighbor[p][j],false });
										rightDel[uNeighbor[p][j]] = true;
										bh.push_back({ uNeighbor[p][j],false });
									}
								}
						}
						else {
							for (int j = 0; j < vNeighbor[p].size(); j++)
								if (!leftDel[vNeighbor[p][j]]) {
									degL[vNeighbor[p][j]]--;
									if (degL[vNeighbor[p][j]] == alpha) {
										bfsQ.push_back({ vNeighbor[p][j],true });
										leftDel[vNeighbor[p][j]] = true;
										bh.push_back({ vNeighbor[p][j],true });
									}
								}
						}
					}
					bfsQ.clear();
				}
				else {
					nextRemain[nextNum] = u;
					nextNum++;
				}
			}
		}
		order.push_back(bh);
		bh.clear();
		swap(remain, nextRemain);
		num = nextNum;
		if (nextNum == 0)
			break;
	}
	return 0;
}


void BiGraph::GobalBasedIndex(int alpha, int beta, int q, bool isU, vector<int> QK,
								vector<int>& uResults, vector<int>& vResults) {

	vector<int> uVis(n1 + 1, false);
	vector<int> vVis(n2 + 1, false);
	vector<int> degU(n1 + 1, 0), degV = vDeg;
	vector<int> uCore, vCore;
	queue<int> qu, qv;

	for(auto &k:QK)
		for (auto& v : IL[k]) {
			if(vVis[v])
				continue;
			if (vAlphaOffset[v].size() > alpha - 1 && vAlphaOffset[v][alpha - 1] >= beta) {
				vVis[v] = true;
				vCore.push_back(v);
				for (auto& u : vNeighbor[v]) {
					if (uAlphaOffset[u].size() < alpha - 1 || uAlphaOffset[u][alpha - 1] < beta) {
						degV[v]--;
						if (degV[v] < beta) {
							qv.push(v);
							vVis[v] = false;
						}
						continue;
					}
					if (!uVis[u]) {
						uVis[u] = true;
						uCore.push_back(u);
					}
					degU[u]++;
				}
			}
		}

	for (int u : uCore) {
		if (degU[u] < alpha) {
			qu.push(u);
			uVis[u] = false;
		}
	}

	while (!qu.empty() || !qv.empty()) {
		while (!qu.empty()) {
			int u = qu.front();
			qu.pop();
			for (int v : uNeighbor[u]) {
				if (vVis[v]) {
					--degV[v];
					if (degV[v] < beta) {
						qv.push(v);
						vVis[v] = false;
					}
				}
			}
		}
		while (!qv.empty()) {
			int v = qv.front(); qv.pop();
			for (int u : vNeighbor[v]) {
				if (uVis[u]) {
					--degU[u];
					if (degU[u] < alpha) {
						qu.push(u);
						uVis[u] = false;
					}
				}
			}
		}
	}

	bool qOK = isU ? uVis[q] : vVis[q];

	if (qOK) {
		queue<pair<int, bool>> bfs;
		bfs.push(make_pair(q, isU));
		if (isU) uResults[q] = 1; else vResults[q] = 1;
		while (!bfs.empty()) {
			pair<int, bool> cur = bfs.front();
			bfs.pop();
			if (cur.second) {
				for (int v : uNeighbor[cur.first])
					if (vVis[v] && !vResults[v]) {
						vResults[v] = 1;
						bfs.push(make_pair(v, false));
					}
			}
			else {
				for (int u : vNeighbor[cur.first]) {
					if (uVis[u] && !uResults[u]) {
						uResults[u] = 1;
						bfs.push(make_pair(u, true));
					}
				}
			}
		}
	}
}

void BiGraph::LocalBasedIndex(int alpha, int beta, int q, bool isU, vector<int> QK,
	vector<int>& uResults, vector<int>& vResults) {

	vector<int> uVis(n1 + 1, false);
	vector<int> vVis(n2 + 1, false);
	vector<int> degU(n1 + 1, 0), degV = vDeg;
	queue<pair<int, bool>> bfs;
	queue<int> qu, qv;

	for (auto& k : QK)
		for (auto& v : IL[k]) {
			if (vVis[v])
				continue;
			if (vAlphaOffset[v].size() > alpha - 1 && vAlphaOffset[v][alpha - 1] >= beta) {
				vVis[v] = true;
				for (auto& u : vNeighbor[v]) {
					if (uAlphaOffset[u].size() < alpha - 1 || uAlphaOffset[u][alpha - 1] < beta) {
						degV[v]--;
						if (degV[v] < beta) {
							qv.push(v);
							vVis[v] = false;
						}
						continue;
					}
					if (!uVis[u]) 
						uVis[u] = true;
					degU[u]++;
				}
			}
		}

	if (isU) {
		if (!uVis[q])
			return;
		else {
			bfs.push(make_pair(q, true));
			uResults[q] = 1;
		}
	}
	else {
		if (!vVis[q])
			return;
		else {
			bfs.push(make_pair(q, false));
			vResults[q] = 1;
		}
	}

	while (!bfs.empty()) {
		pair<int, bool> cur = bfs.front();
		bfs.pop();
		if (cur.second) {
			if (uResults[cur.first] == -1)
				continue;
			if (degU[cur.first] >= alpha) {
				for (int v : uNeighbor[cur.first]) {
					if (vVis[v] && vResults[v] == 0) {
						if (degV[v] >= beta) {
							bfs.push(make_pair(v, false));
							vResults[v] = 1;
						}
						else {
							qv.push(v);
							vResults[v] = -1;
							vVis[v] = false;
							DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
						}
					}
				}
			}
			else {
				qu.push(cur.first);
				uResults[cur.first] = -1;
				uVis[cur.first] = false;
				DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
			}
		}
		else {
			if (vResults[cur.first] == -1)
				continue;
			if (degV[cur.first] >= beta) {
				for (int u : vNeighbor[cur.first]) {
					if (uVis[u] && uResults[u] == 0) {
						if (degU[u] >= alpha) {
							bfs.push(make_pair(u, true));
							uResults[u] = 1;
						}
						else {
							qu.push(u);
							uResults[u] = -1;
							uVis[u] = false;
							DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
						}
					}
				}
			}
			else {
				qv.push(cur.first);
				vResults[cur.first] = -1;
				vVis[cur.first] = false;
				DeleteCand(alpha, beta, uResults, vResults, degU, degV, qu, qv);
			}
		}
	}

	bool isQ = true;
	if (isU && uResults[q] == -1)
		isQ = false;
	else if (!isU && vResults[q] == -1)
		isQ = false;
	if (!isQ) {
		for (int i = 0; i <= n1; i++) uResults[i] = 0;
		for (int i = 0; i <= n2; i++) vResults[i] = 0;
	}
}

void BiGraph::LocalBasedSearch2(int alpha, int beta, int q, bool isU, vector<int> QK,
	vector<int>& uResults, vector<int>& vResults) {

	vector<int> degU = uDeg, degV = vDeg;
	queue<pair<int, bool>> bfs;
	queue<int> qu, qv;

	if (alpha <= maxK) {
		if (isU && uAlphaOffset[q][alpha - 1] >= beta) {
			bfs.push(make_pair(q, isU));
			uResults[q] = 1;
		}
		else if (!isU && vAlphaOffset[q][alpha - 1] >= beta && checkKeywords2(q, QK)) {
			bfs.push(make_pair(q, isU));
			vResults[q] = 1;
		}

		while (!bfs.empty()) {
			pair<int, bool> cur = bfs.front();
			bfs.pop();
			if (cur.second) {
				if (uResults[cur.first] == -1)
					continue;
				if (degU[cur.first] >= alpha) {
					for (int v : uNeighbor[cur.first]) {
						if (vAlphaOffset[v].size() > alpha -1 && vAlphaOffset[v][alpha - 1] >= beta
							&& degV[v] >= beta && checkKeywords2(v, QK) && vResults[v] == 0) {
							bfs.push(make_pair(v, false));
							vResults[v] = 1;
						}
						else if (vResults[v] == 0) {
							qv.push(v);
							vResults[v] = -1;
							DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
						}
					}
				}
				else {
					qu.push(cur.first);
					uResults[cur.first] = -1;
					DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
				}
			}
			else {
				if (vResults[cur.first] == -1)
					continue;
				if (degV[cur.first] >= beta) {
					for (int u : vNeighbor[cur.first]) {
						if (uAlphaOffset[u].size()>alpha - 1 && uAlphaOffset[u][alpha - 1] >= beta
							&& degU[u] >= alpha && uResults[u] == 0) {
							bfs.push(make_pair(u, true));
							uResults[u] = 1;
						}
						else if (uResults[u] == 0) {
							qu.push(u);
							uResults[u] = -1;
							DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
						}
					}
				}
				else {
					qv.push(cur.first);
					vResults[cur.first] = -1;
					DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
				}
			}
		}
	}
	else {
		if (isU && uBetaOffset[q][beta - 1] >= alpha) {
			bfs.push(make_pair(q, isU));
			uResults[q] = 1;
		}
		else if (!isU && vBetaOffset[q][beta - 1] >= alpha && checkKeywords2(q, QK)) {
			bfs.push(make_pair(q, isU));
			vResults[q] = 1;
		}

		while (!bfs.empty()) {
			pair<int, bool> cur = bfs.front();
			bfs.pop();
			if (cur.second) {
				if (uResults[cur.first] == -1)
					continue;
				if (degU[cur.first] >= alpha) {
					for (int v : uNeighbor[cur.first]) {
						if (vBetaOffset[v].size()>beta - 1 && vBetaOffset[v][beta - 1] >= alpha
							&& degV[v] >= beta && checkKeywords2(v, QK) && vResults[v] == 0) {
							bfs.push(make_pair(v, false));
							vResults[v] = 1;
						}
						else if (vResults[v] == 0) {
							qv.push(v);
							vResults[v] = -1;
							DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
						}
					}
				}
				else {
					qu.push(cur.first);
					uResults[cur.first] = -1;
					DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
				}
			}
			else {
				if (vResults[cur.first] == -1)
					continue;
				if (degV[cur.first] >= beta) {
					for (int u : vNeighbor[cur.first]) {
						if (uBetaOffset[u].size()>beta - 1 && uBetaOffset[u][beta - 1] >= alpha
							&& degU[u] >= alpha && uResults[u] == 0) {
							bfs.push(make_pair(u, true));
							uResults[u] = 1;
						}
						else if (uResults[u] == 0) {
							qu.push(u);
							uResults[u] = -1;
							DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
						}
					}
				}
				else {
					qv.push(cur.first);
					vResults[cur.first] = -1;
					DeleteCand2(alpha, beta, uResults, vResults, degU, degV, qu, qv);
				}
			}
		}
	}

	bool isQ = true;
	if (isU && uResults[q] == -1)
		isQ = false;
	else if (!isU && vResults[q] == -1)
		isQ = false;
	if (!isQ) {
		for (int i = 0; i <= n1; i++) uResults[i] = 0;
		for (int i = 0; i <= n2; i++) vResults[i] = 0;
	}
}

bool BiGraph::checkKeywords2(int v, vector<int> &QK) {
	for (int k : keyWords[v])
		if (QK[k]==1)
			return true;
	return false;
}


void BiGraph::DeleteCand2(int alpha, int beta, vector<int>& uResults, vector<int>& vResults,
	vector<int>& degU, vector<int>& degV, queue<int>& qu, queue<int>& qv) {

	while (!qu.empty() || !qv.empty()) {
		while (!qu.empty()) {
			int u = qu.front();
			qu.pop();
			for (int v : uNeighbor[u]) {
				--degV[v];
				if (vResults[v] == 1) {
					if (degV[v] < beta) {
						qv.push(v);
						vResults[v] = -1;
					}
				}
			}
		}
		while (!qv.empty()) {
			int v = qv.front(); qv.pop();
			for (int u : vNeighbor[v]) {
				--degU[u];
				if (uResults[u] == 1) {
					if (degU[u] < alpha) {
						qu.push(u);
						uResults[u] = -1;
					}
				}
			}
		}
	}
}

void BiGraph::DeleteCand3(int alpha, int beta, vector<int>& uVis, vector<int>& vVis,
	vector<int>& degU, vector<int>& degV, queue<int>& qu, queue<int>& qv) {

	while (!qu.empty() || !qv.empty()) {
		while (!qu.empty()) {
			int u = qu.front();
			qu.pop();
			for (int v : uNeighbor[u]) {
				if (vVis[v]) {
					--degV[v];
					if (degV[v] < beta) {
						qv.push(v);
						vVis[v] = false;
					}
				}
			}
		}
		while (!qv.empty()) {
			int v = qv.front(); qv.pop();
			for (int u : vNeighbor[v]) {
				if (uVis[u]) {
					--degU[u];
					if (degU[u] < alpha) {
						qu.push(u);
						uVis[u] = false;
					}
				}
			}
		}
	}
}

void BiGraph::DeleteCandForCore(int alpha, int beta, vector<int>& uResults, vector<int>& vResults, vector<int>& uVis, vector<int>& vVis,
	vector<int>& degU, vector<int>& degV, queue<int>& qu, queue<int>& qv) {

	while (!qu.empty() || !qv.empty()) {
		while (!qu.empty()) {
			int u = qu.front();
			qu.pop();
			for (int v : uNeighbor[u]) {
				if (vVis[v]) {
					if (degV[v] == -1) {
						degV[v] = 0;
						for (int& u : vNeighbor[v])
							if (uVis[u])
								degV[v]++;
					}
					else
						--degV[v];
					if (degV[v] < beta) {
						qv.push(v);
						vVis[v] = false;
						vResults[v] = -1;
					}
				}
			}
		}
		while (!qv.empty()) {
			int v = qv.front(); qv.pop();
			for (int u : vNeighbor[v]) {
				if ( uVis[u] ) {
					if (degU[u] == -1) {
						degU[u] = 0;
						for (int& nbr : uNeighbor[u])
							if (vVis[nbr])
								degU[u]++;
					}
					else
						--degU[u];
					if (degU[u] < alpha) {
						qu.push(u);
						uVis[u] = false;
						uResults[u] = -1;
					}
				}
			}
		}
	}
}