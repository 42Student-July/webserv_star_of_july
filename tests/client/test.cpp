#include "test.hpp"

void testfile_setter(T_TEST &test, const std::string &request_file_path)
{
	test.request_file_path = request_file_path;
	test.request = readFile(test.request_file_path);
	std::string response_path = "expected_response/" + test.request_file_path.substr(8) + ".res";
	test.expected_response = readFile(response_path);
}


// テスト方法2種類(TEST1方式とTEST2方式)
// TEST1: request, expected_responseを直接入れる(readFileでファイルを読み込んでも良い)
// TEST2: requestのパス名をセットすればうまくやってくれる(事前にexpected_responseフォルダ内に.resファイルの準備が必要)
// ※ NGの場合：TEST1ではresponseとexpected_responseを標準出力、TEST2ではdiffを標準出力
T_TEST TEST1() {
	T_TEST test;
	test.request = readFile("request/curl.crlf");
	test.expected_response = readFile("expected_response/curl.crlf.res");
	return test;
}

T_TEST TEST2() {
	T_TEST test;
	testfile_setter(test, "request/curl.crlf");
	return test;
}

T_TEST TEST3() {
	T_TEST test;
	testfile_setter(test, "request/chrome.crlf");
	return test;
}

std::vector<T_TEST> makeTests() {
	std::vector<T_TEST> tests;

	tests.push_back(TEST1());
	tests.push_back(TEST2());
	tests.push_back(TEST3());
	
	return tests;
}
