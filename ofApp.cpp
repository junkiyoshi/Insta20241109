#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(39);

	this->font_size = 50;
	this->font.loadFont("fonts/msgothic.ttc", this->font_size, true, true, true);

	this->charactor_list = { ';', ')', '(', 'y', 'g', 'o', 'l', 'o', 'h', 't', 'n', 'a' };
}

//--------------------------------------------------------------
void ofApp::update() {

	float radius = 280;
	for (int i = 0; i < 3; i++) {

		int deg = ofGetFrameNum() * 2 + i * 120;
		int next_deg = ofGetFrameNum() * 2 + i * 120 + 1;

		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		auto next = glm::vec2(radius * cos(next_deg * DEG_TO_RAD), radius * sin(next_deg * DEG_TO_RAD));

		if (ofGetFrameNum() % 2 == 0) {

			vector<glm::vec2> log;
			log.push_back(location);
			this->log_list.push_back(log);

			auto deg = glm::atan(next.y - location.y, next.x - location.x) * RAD_TO_DEG + 180 + ofRandom(-30, 30);
			glm::vec2 velocity = glm::vec2(cos(deg * DEG_TO_RAD), sin(deg * DEG_TO_RAD));
			this->velocity_list.push_back(velocity);

			this->noise_seed_list.push_back(ofRandom(1000));

			this->index_list.push_back((ofGetFrameNum() / 2) % this->charactor_list.size());
		}
	}

	for (int i = 0; i < this->log_list.size(); i++) {

		auto future = this->velocity_list[i] * 200;
		auto random_deg = ofMap(ofNoise(glm::vec2(this->noise_seed_list[i], ofGetFrameNum() * 0.01)), 0, 1, -30, 30);
		future += glm::vec2(10 * cos(random_deg * DEG_TO_RAD), 10 * sin(random_deg * DEG_TO_RAD));

		auto next = this->log_list[i].back() + glm::normalize(future) * 10;
		this->log_list[i].push_back(next);

		if (this->log_list[i].size() > 50) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->noise_seed_list.erase(this->noise_seed_list.begin() + i);
			this->index_list.erase(this->index_list.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (int i = 0; i < this->log_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->log_list[i].back());

		auto rad = atan2(this->velocity_list[i].y, this->velocity_list[i].x);
		ofRotate(rad * RAD_TO_DEG + 270);
		
		ofPath path = this->font.getCharacterAsPoints(this->charactor_list[this->index_list[i]], true, false);
		vector<ofPolyline> outline = path.getOutline();

		ofFill();
		ofSetColor(this->log_list[i].size() < 10 ? 255 : ofMap(this->log_list[i].size(), 10, 50, 255, 39));
		ofBeginShape();
		for (int outline_index = 0; outline_index < outline.size(); outline_index++) {

			if (outline_index != 0) { ofNextContour(true); }

			auto vertices = outline[outline_index].getResampledByCount(200).getVertices();
			for (auto& vertex : vertices) {

				auto location = vertex - glm::vec3(this->font_size * 0.5, this->font_size * -0.5, 330);
				ofVertex(location);
			}
		}
		ofEndShape(true);


		ofPopMatrix();
	}

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}