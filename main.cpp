#include"utility.h"
#include"bigraph.h"

typedef struct query {
	int alpha;
	int beta;
	int q;
	bool isU;
	vector<int> QK;
}Query;


void readQuery(string path, vector<Query> &queryList) {
	FILE* fp = fopen(path.c_str(), "r");
	if (!fp) {
		printf("Failed to open query file: %s\n", path.c_str());
		return;
	}
	int a, b, q, type;
	while (fscanf(fp, "%d %d %d %d", &a, &b, &q, &type) == 4) {
		Query qry;
		qry.alpha = a;
		qry.beta = b;
		qry.q = q;
		qry.isU = (type == 0);
		int ch;
		while (1) {
			// skip spaces and tabs
			ch = fgetc(fp);
			if (ch == '\n' || ch == EOF) 
				break;
			if (ch == ' ' || ch == '\t' || ch == '\r') 
				continue;
			ungetc(ch, fp);
			int kw;
			if (fscanf(fp, "%d", &kw) == 1) {
				qry.QK.push_back(kw);
			}
			else break;
		}
		queryList.push_back(qry);
		if (ch == EOF) 
			break;
	}
	fclose(fp);
}

void Global(BiGraph &graph, vector<Query> &queryList);
void Local(BiGraph& graph, vector<Query>& queryList);
void Local2(BiGraph& graph, vector<Query>& queryList);
void GlobalInedx(BiGraph& graph, vector<Query>& queryList);
void LocalInedx(BiGraph& graph, vector<Query>& queryList);

double runGlobal(BiGraph& graph, Query& query);
double runLocal(BiGraph& graph, Query& query);
double runGlobalInedx(BiGraph& graph, Query& query);
double runLocalInedx(BiGraph& graph, Query& query);

int main(int argc, char* argv[]) {
	string dataset = "../BData/DBLP";
	BiGraph graph(dataset);

	//graph.BulidIndex();

	vector<Query> queryList;
	readQuery(dataset + "/query.txt", queryList);
	printf("Loaded %d queries\n", (int)queryList.size());

	auto start = chrono::system_clock::now();
	graph.BulidIndex();
	auto end = chrono::system_clock::now();
	chrono::duration<double> time = end - start;

	double space = 0;
	for (auto& i : graph.IL)
		space += i.size();
	for (auto& i : graph.uAlphaOffset)
		space += i.size();
	for (auto& i : graph.vAlphaOffset)
		space += i.size();
	for (auto& i : graph.uBetaOffset)
		space += i.size();
	for (auto& i : graph.vBetaOffset)
		space += i.size();

	cout <<"Bulid time: " << time.count()  << " Size: "<< space * sizeof(int) / (1024 * 1024) << endl;

	//for (int i = 0; i < 1; i++) {
	//	Global(graph, queryList);
	//	//LocalBasedSearch(graph, queryList);
	//	Local2(graph, queryList);
	//	GlobalInedx(graph, queryList);
	//	LocalInedx(graph, queryList);
	//}
	int c = 0;
	for (auto query : queryList) {
		double time1 = runGlobal(graph, query);
		double time2 = runLocal(graph, query);
		double time3 = runGlobalInedx(graph, query);
		double time4 = runLocalInedx(graph, query);
		cout << query.alpha << " " << query.beta << " : " << time1 << " " << 
			time2 << " " << time3 << " "<< time4 << endl;
		c++;
		if (c % 5 == 0)
			cout << endl;
	}

	return 0;
}

void Global(BiGraph& graph, vector<Query>& queryList) {
	auto start = chrono::system_clock::now();
	auto end = chrono::system_clock::now();
	chrono::duration<double> time;

	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);

	start = chrono::system_clock::now();
	for (auto i : queryList) {
		fill(uResults.begin(), uResults.end(), 0);
		fill(vResults.begin(), vResults.end(), 0);
		graph.Global(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults);
		int count1 = 0, count2 = 0;
		for (auto i : uResults) if (i==1) count1++;
		for (auto i : vResults) if (i==1) count2++;
		cout << "Verification: " << graph.Verification(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults) << "   "  << count1 << " " << count2 << endl;
	}
	end = chrono::system_clock::now();
	time = end - start;
	printf("Global Time: %.3lf ms\n", time.count() * 1000);
}

void Local(BiGraph& graph, vector<Query>& queryList) {
	auto start = chrono::system_clock::now();
	auto end = chrono::system_clock::now();
	chrono::duration<double> time;

	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);

	start = chrono::system_clock::now();
	for (auto i : queryList) {
		fill(uResults.begin(), uResults.end(), 0);
		fill(vResults.begin(), vResults.end(), 0);
		graph.Local(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults);
		//int count1 = 0, count2 = 0;
		//for (auto i : uResults) if (i==1) count1++;
		//for (auto i : vResults) if (i==1) count2++;
		//cout << "Verification: "<< graph.Verification2(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults) << "   " << count1 << " " << count2 << endl;
	}
	end = chrono::system_clock::now();
	time = end - start;
	printf("Local Time: %.3lf ms\n\n", time.count() * 1000);
}

void Local2(BiGraph& graph, vector<Query>& queryList) {
	auto start = chrono::system_clock::now();
	auto end = chrono::system_clock::now();
	chrono::duration<double> time;

	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);

	start = chrono::system_clock::now();
	for (auto i : queryList) {
		fill(uResults.begin(), uResults.end(), 0);
		fill(vResults.begin(), vResults.end(), 0);
		graph.Local2(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults);
		//int count1 = 0, count2 = 0;
		//for (auto i : uResults) if (i == 1) count1++;
		//for (auto i : vResults) if (i == 1) count2++;
		//cout << "Verification: " << graph.Verification2(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults) << "   " << count1 << " " << count2 << endl;
	}
	end = chrono::system_clock::now();
	time = end - start;
	printf("Local2 Time: %.3lf ms\n", time.count() * 1000);
}

void GlobalInedx(BiGraph& graph, vector<Query>& queryList) {

	auto start = chrono::system_clock::now();
	auto end = chrono::system_clock::now();
	chrono::duration<double> time;

	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);

	start = chrono::system_clock::now();
	for (auto i : queryList) {

		/*int c = 0;
		for (int v = 1; v <= graph.n2; v++)
			if (graph.vDeg[v] >= i.beta && graph.checkKeywords(v, i.QK))
				c++;
		cout << c << endl;*/

		fill(uResults.begin(), uResults.end(), 0);
		fill(vResults.begin(), vResults.end(), 0);
		graph.GobalBasedIndex(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults);
		//int count1 = 0, count2 = 0;
		//for (auto i : uResults) if (i==1) count1++;
		//for (auto i : vResults) if (i==1) count2++;
		//cout << "Verification: " << graph.Verification2(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults) << "   "  << count1 << " " << count2 << endl;
	}
	end = chrono::system_clock::now();
	time = end - start;
	printf("GlobalIndex Time: %.3lf ms\n", time.count() * 1000);
}

void LocalInedx(BiGraph& graph, vector<Query>& queryList) {
	
	//graph.BulidIndex();
	
	auto start = chrono::system_clock::now();
	auto end = chrono::system_clock::now();
	chrono::duration<double> time;

	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);

	start = chrono::system_clock::now();
	for (auto i : queryList) {
		fill(uResults.begin(), uResults.end(), 0);
		fill(vResults.begin(), vResults.end(), 0);
		graph.LocalBasedIndex(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults);
		//int count1 = 0, count2 = 0;
		//for (auto i : uResults) if (i==1) count1++;
		//for (auto i : vResults) if (i==1) count2++;
		//cout << "Verification: " << graph.Verification2(i.alpha, i.beta, i.q, i.isU, i.QK, uResults, vResults) << "   "  << count1 << " " << count2 << endl;
	}
	end = chrono::system_clock::now();
	time = end - start;
	printf("LocalInedx Time: %.3lf ms\n\n", time.count() * 1000);
}

double runGlobal(BiGraph& graph, Query& query) {
	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);
	auto start = chrono::system_clock::now();
	graph.Global(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults);
	//int count1 = 0, count2 = 0;
	//for (auto i : uResults) if (i==1) count1++;
	//for (auto i : vResults) if (i==1) count2++;
	//cout << "Verification: " << graph.Verification(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults) << "   "  << count1 << " " << count2 << endl;
	auto end = chrono::system_clock::now();
	chrono::duration<double> time = end - start;
	return time.count() * 1000;
}

double runLocal(BiGraph& graph, Query& query) {
	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);
	auto start = chrono::system_clock::now();
	graph.Local2(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults);
	//int count1 = 0, count2 = 0;
	//for (auto i : uResults) if (i==1) count1++;
	//for (auto i : vResults) if (i==1) count2++;
	//cout << "Verification: " << graph.Verification2(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults) << "   "  << count1 << " " << count2 << endl;
	auto end = chrono::system_clock::now();
	chrono::duration<double> time = end - start;
	return time.count() * 1000;
}

double runGlobalInedx(BiGraph& graph, Query& query) {
	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);
	auto start = chrono::system_clock::now();
	graph.GobalBasedIndex(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults);
	//int count1 = 0, count2 = 0;
	//for (auto i : uResults) if (i==1) count1++;
	//for (auto i : vResults) if (i==1) count2++;
	//cout << "Verification: " << graph.Verification2(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults) << "   "  << count1 << " " << count2 << endl;
	auto end = chrono::system_clock::now();
	chrono::duration<double> time = end - start;
	return time.count() * 1000;
}

double runLocalInedx(BiGraph& graph, Query& query) {
	vector<int> uResults(graph.n1 + 1, 0);
	vector<int> vResults(graph.n2 + 1, 0);
	auto start = chrono::system_clock::now();
	graph.LocalBasedIndex(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults);
	//int count1 = 0, count2 = 0;
	//for (auto i : uResults) if (i==1) count1++;
	//for (auto i : vResults) if (i==1) count2++;
	//cout << "Verification: " << graph.Verification2(query.alpha, query.beta, query.q, query.isU, query.QK, uResults, vResults) << "   "  << count1 << " " << count2 << endl;
	auto end = chrono::system_clock::now();
	chrono::duration<double> time = end - start;
	return time.count() * 1000;
}