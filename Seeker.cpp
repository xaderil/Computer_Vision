#include "Seeker.hpp"

const int Seeker::MIN_OBJECT_AREA = 20 * 20;
vector<Point> Seeker::circle_contour = {Point(100, 100), Point(200,100), Point(200, 200), Point(100, 200)};


/// В конструкторе инициализируем массивы для графиков
Seeker::Seeker() {
};
/// Поиск объекта, определение его границ и координат центра
bool Seeker::findObject(cv::Mat* Output_frame, cv::Mat* BGR_frame, int FRAME_WIDTH, int FRAME_HEIGHT) {

	// Поиск контура
	Output_frame->copyTo(frame);
	contourPoints = 0;
	contourPoints = Mat::zeros(BGR_frame->size().height, BGR_frame->size().width, CV_8UC1);
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(frame, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
	if ((hierarchy.size() == 0) || (hierarchy.size() == -1)) {
		cout << "Контуры не найдены" << endl;
		return false;
	}; 
	bool objectFound = false;
	vector<int> areas(contours.size());
	vector<int> xPos(contours.size());
	vector<int> yPos(contours.size());
	if (hierarchy.size() < 100)
	{
		/// 
		for (int index = 0; index < contours.size(); index++) {

			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00;
			xPos[index] = moment.m10 / area;
			yPos[index] = moment.m01 / area;
			if	(area > 200000) 
			{ // Отсев слишком большого объекта
				areas[index] = 0;
				continue;
			};
			areas[index] = area;
		};

		// Поиск максимального элемента и его индекса в векторе areas
		int Max_area = *std::max_element(begin(areas), end(areas));
		int Max_area_index = distance(areas.begin(), find(areas.begin(), areas.end(), Max_area));


		/// Если площадь объекта больше минимальной, то запоминаем и отрисовываем контур
		if (Max_area > MIN_OBJECT_AREA)
		{	

			// СonvexHull контура
			vector< vector<Point> > hull(contours.size());
			convexHull(contours[Max_area_index], hull[Max_area_index]);
			this->contour = hull[Max_area_index];

			// Проверка на близость с границей
			bool notAroundBorder = checkContourBorder(FRAME_WIDTH, FRAME_HEIGHT);

			if (notAroundBorder) {
				// Поиск и отрисовка прямоугольника, получение его меньшей стороны
				RotatedRect box = minAreaRect(hull[Max_area_index]);
				box.points(this->vtx);
				// this->rectPoints = {vtx[0], vtx[1], vtx[2]}; 
				for (int i = 0; i < 4; i++) {
					line(*BGR_frame, vtx[i], vtx[(i + 1) % 4], Scalar(0, 255, 0), 1, LINE_AA);
				};
				float RotRectWidth = box.size.width;
				float RotRectHeight = box.size.height; 
				float smallest_side = min(RotRectHeight, RotRectWidth);

				this->pxX = xPos[Max_area_index];
				this->pxY = yPos[Max_area_index];
				this->pxDiameter = smallest_side;

				return true;

			} else {

				return false;
				
			};
		} 
		else 
		{
			return false;
		};
	} 
	else 
	{
		cout << "Слишком много шума" << endl;
		return false;
	};
};

/// Отрисовка контура объекта, центра, вставка текста
void Seeker::drawObject(Mat *BGR_frame, float xPos, float yPos, float zPos, int xPosPx, int yPosPx) {
	vector<vector<Point>> cont = {contour};
	drawContours(*BGR_frame, cont, 0, Scalar(0, 0, 255), 2, 8);
	circle(*BGR_frame, Point(xPosPx, yPosPx), 3, cv::Scalar(0, 0, 255));
	putText(*BGR_frame, to_string(int(xPos*1000)) + " , " + to_string(int(yPos*1000)) + " , " + to_string(int(zPos*1000)), Point(xPosPx, yPosPx + 20), 1, 1, Scalar(255, 255, 0));
	
	drawContours(contourPoints, cont, 0, Scalar(255, 255, 255), -1, 8);
	findNonZero(contourPoints, nonZeroCoordinates);
};
	
/// Возврат FALSE если контур мячика возле границы кадра
bool Seeker::checkContourBorder(int FRAME_WIDTH, int FRAME_HEIGHT) {
	for (Point point : this->contour) {
		if (abs((point.x) - FRAME_WIDTH) < 3 || abs((point.y) - FRAME_HEIGHT) < 3) {
			return false;
		} else if (abs((point.x) - FRAME_WIDTH) > (FRAME_WIDTH-3) || abs((point.y) - FRAME_HEIGHT) > (FRAME_HEIGHT-3)) {
			return false;
		} else {
			continue;
		}
	};
	return true;
};

cv::Mat Seeker::getBallImage() {

	return this->contourPoints;

};


int Seeker::get_pxX() {
	return this->pxX;
};
int Seeker::get_pxY() {
	return this->pxY;
};
int Seeker::get_pxDiameter() {
	return this->pxDiameter;
};



