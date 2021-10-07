#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetBackgroundAuto(false); 
	ofBackground(255); 
	ofSetLogLevel(OF_LOG_VERBOSE);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	if (draw_flag) {
		ofSetColor(0); // ������ 
		ofFill(); // �� ä���
		ofSetLineWidth(5); // ���� 5
		for (i = 0; i < line_num * 2; i += 2) { // 
			ofDrawLine(lx[i], ly[i], lx[i + 1], ly[i + 1]);
		}

		for (i = 0; i < dot_num; i++) { 
			if (i == 0) { 
				target_dot = i; 
				ofSetColor(255, 0, 0); // ������
			}
			else {
				ofSetColor(0x000000); // ������
			}
			ofDrawCircle(dx[i], dy[i], 10);
		}
		draw_flag = 0;
	}

	else if (!draw_flag) return;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) { 

	if (key == 'd' || key == 'D') { 
		draw_flag = 1; // draw_flag�� set
		draw(); 
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}


	if (key == OF_KEY_LEFT) { 
		ofFill(); // ä���
		ofSetColor(0x000000);
		ofDrawCircle(dx[target_dot], dy[target_dot], 10); // ����������
		target_dot = (target_dot + dx.size() - 1) % dx.size();
		ofSetColor(255, 0, 0); //������
		ofDrawCircle(dx[target_dot], dy[target_dot], 10);
		cout << "Selected Dot coordinate is (" << dx[target_dot] << ", " << dy[target_dot] << ")" << endl;
	}
	if (key == OF_KEY_RIGHT) {
		ofFill();
		ofSetColor(0);
		ofDrawCircle(dx[target_dot], dy[target_dot], 10); 
		target_dot = (target_dot + 1) % dx.size(); // 1 ����
		ofSetColor(255, 0, 0); // ������
		ofDrawCircle(dx[target_dot], dy[target_dot], 10);
		cout << "Selected Dot coordinate is (" << dx[target_dot] << ", " << dy[target_dot] << ")" << endl;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	if (key == 'l' || key == 'L') {
		ofFileDialogResult openFileResult = ofSystemLoadDialog("Select an input file");
		if (openFileResult.bSuccess) {
			ofLogVerbose("User selected a file");
			processOpenFileSelection(openFileResult); // ���Ϸκ��� �� �Է¹ޱ� 
		}
		else {
			ofLogVerbose("User hit cancel");
		}
	}
	if (key == 'q' || key == 'Q') {
		lx.clear(); ly.clear();
		vector<int>().swap(lx);
		vector<int>().swap(ly);
		cout << "Memory for line segment has been freed" << endl;
		dx.clear(); dy.clear();
		vector<int>().swap(dx);
		vector<int>().swap(dy);
		cout << "Memory for dot has been freed" << endl;
		return;
	}
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
	ifstream file;
	file.open(openFileResult.getPath()); // �ش� ��ο� �ִ� ���� ����

	int line_or_dot = 0; 
	int line_count=0;
	string line;

	if (!file.is_open()) cout << "file open error" << endl;
	else cout << "We found the target" << endl;

	while (!file.eof()) {
		getline(file, line); 
		vector <string> words = ofSplitString(line, " "); // split

		words.resize(100);

		if (words.size() == 1) { // ���� ���� or �� ����
			if (line_or_dot == 0) { 
				line_num = atoi(words[0].c_str()); 
				line_count = line_num;
				cout << "The number of line is: " << line_num << endl;
			}
			else { // ��
				dot_num = atoi(words[0].c_str()); // ���� ����
				cout << "The number of dot is: " << dot_num << endl;
			}
		}
		else if (words.size() > 1) { // ��ǥ��
			if (line_or_dot == 0) {
				lx.push_back(atoi(words[0].c_str())); 
				ly.push_back(atoi(words[1].c_str()));
				lx.push_back(atoi(words[2].c_str()));
				ly.push_back(atoi(words[3].c_str()));

				line_count--;

			}
			else { 
				dx.push_back(atoi(words[0].c_str())); 
				dy.push_back(atoi(words[1].c_str()));
			}
		}
		if (line_count == 0) { 
			line_or_dot = 1;
		}
	}
}