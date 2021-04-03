#include "Seeker.hpp"

const int Seeker::MIN_OBJECT_AREA = 20 * 20;
int Seeker::num_of_chart_data = 0;

/// В конструкторе инициализируем массивы для графиков
Seeker::Seeker(int window_size) {

	datasize = window_size;
	ydata.resize(datasize);
	pointer_ydata = &ydata;
	xdata.resize(datasize);
	pointer_xdata = &xdata;
	for(int i = 0; i<datasize; i++) {
		xdata[i] = i + 1;
	};
};
/// Поиск объекта, определение его границ и координат центра
bool Seeker::findObject(cv::Mat* Output_frame, cv::Mat* BGR_frame) {

	// Поиск контура
	Output_frame->copyTo(frame);
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

			// Поиск и отрисовка прямоугольника, получение его меньшей стороны
			Point2f vtx[4];
			RotatedRect box = minAreaRect(hull[Max_area_index]);
			box.points(vtx);
			for (int i = 0; i < 4; i++) {
				line(*BGR_frame, vtx[i], vtx[(i + 1) % 4], Scalar(0, 255, 0), 1, LINE_AA);
			};
			float RotRectWidth = box.size.width;
			float RotRectHeight = box.size.height; 
			float smallest_side = min(RotRectHeight, RotRectWidth);
			// Отрисовка контура объекта, центра, вставка текста
			// drawContours(*BGR_frame, hull, Max_area_index, Scalar(0, 0, 255), 2, 8);
			// circle(*BGR_frame, Point(xPos[Max_area_index], yPos[Max_area_index]), 3, cv::Scalar(0, 0, 255));
			// putText(*BGR_frame, to_string(xPos[Max_area_index]) + " , " + to_string(xPos[Max_area_index]), Point(xPos[Max_area_index], yPos[Max_area_index] + 20), 1, 1, Scalar(255, 255, 0));

			// Заполнение массива данных для графика. Сначала заполняем нули числами, потом сдвигаем влево 
			if (num_of_chart_data == (ydata.size() - 1)) {
				Seeker::vectorRotateLeft(this->pointer_ydata, areas[Max_area_index]);
			}
			else {
				ydata[num_of_chart_data] = areas[Max_area_index];
				num_of_chart_data++;
			};

			this->pxX = xPos[Max_area_index];
			this->pxY = yPos[Max_area_index];
			this->pxDiameter = smallest_side;
			this->contour = hull[Max_area_index];
			return true;
		
		} 
		else 
		{
			return false;
		};
	} 
	else 
	{
		return false;
	};
};


void Seeker::drawObject(Mat *BGR_frame, float xPos, float yPos, float zPos, int xPosPx, int yPosPx) {
	vector<vector<Point>> cont = {contour};
	drawContours(*BGR_frame, cont, 0, Scalar(0, 0, 255), 2, 8);
	circle(*BGR_frame, Point(xPosPx, yPosPx), 3, cv::Scalar(0, 0, 255));
	putText(*BGR_frame, to_string(int(xPos*1000)) + " , " + to_string(int(yPos*1000)) + " , " + to_string(int(zPos*1000)), Point(xPosPx, yPosPx + 20), 1, 1, Scalar(255, 255, 0));
};


//// Методы работы с приватными членами класса

vector<double> Seeker::getYData() {
	return this->ydata;
};
vector<double> Seeker::getXData() {
	return this->xdata;
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



//// То, от чего нужно будет избавиться

/// Сдвиг данных влево
void Seeker::vectorRotateLeft(vector<double>* ydata, int last_largest_num)
{
    try {
    for(std::size_t i = 0; i < ydata->size()-1; i++) {
        (*ydata)[i] = (*ydata)[i+1];
    }
    } catch(std::bad_alloc& ex) {
        std::cout << ex.what();
    }
    (*ydata)[ydata->size()-1] = last_largest_num;
};