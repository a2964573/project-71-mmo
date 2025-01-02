#include "main.h"


GLOBAL _global = {0,};

/* 사다리타기 게임
 * arguments
 *   int : leg cols
 *   int : leg rows
 */
int main(int argc, char *argv[])
{
	if(argc < 3) {
		std::cerr << "invalid arguments..." << std::endl;
		return -1;
	}

	if(atoi(argv[1]) < 0 || atoi(argv[2]) < 0) {
		std::cerr << "invalid arguments..." << std::endl;
		return -1;
	}

	_global.cols_cnt = atoi(argv[1]);
	_global.rows_cnt = atoi(argv[2]);
	_global.height   = (_global.rows_cnt * 2) + 2;

	COLITEM  col     = {0,};
	ROWITEM  row     = {0,};
	COLITEM* col_ptr = nullptr;
	ROWITEM* row_ptr = nullptr;

	int cpos;
	for(cpos = 0; cpos < _global.cols_cnt; cpos++) {
		col = {0,};
		col.col_pos  = cpos;

		std::cout << cpos + 1 << "번 아이템을 입력하시오." << std::endl;
		std::getline(std::cin, col.value);

		if(cpos == 0 || cpos == _global.cols_cnt) {
			col.max_rcnt = _global.rows_cnt;
		}
		else {
			col.max_rcnt = _global.rows_cnt * 2;
		}

		_global.cols.push_back(col);
	}

	int rpos;
	col_ptr = getColItemByIndex(0);
	while(true) {
		if(col_ptr == nullptr) {
			break;
		}

		for(rpos = 0; rpos < _global.height; rpos++) {
			row = {0,};

			if(col_ptr->row_cnt >= col_ptr->max_rcnt) {
				row.row_pos = rpos;
			}
			else {
				if(setRowItem(col_ptr->col_pos, rpos, &row) < 0) {
					// 발생하면 안되는 에러
					return -1;
				}
				
				if(row.used == 1) {
					col_ptr->row_cnt++;
				}
			}

			col_ptr->rows.push_back(row);
		}

		col_ptr = getNextColItem(col_ptr->col_pos);
	}

	// LOG
	// printGhostLegAbout();

	std::cout << "생성완료..." << std::endl;

	USER user = {0,};

	int position = 0;
	while(true) {
		user = {0,};

		position = inputNumber("위치를 입력하세요. (0 입력 시 종료)");
		if(position == 0) {
			break;
		}
		else
		if(position == 991130) {
			printGhostLegAbout();
			continue;
		}
		else
		if(position > _global.cols_cnt) {
			std::cout << "입력 값이 큽니다." << std::endl;
			continue;
		}

		user.col_pos = position - 1;
		user.row_pos = 0;

		while(user.row_pos < _global.height) {
			row_ptr = getRowItemByIndex(user.col_pos, user.row_pos);
			if(row_ptr == nullptr) {
				std::cerr << "[PLAY] row_ptr is null (getRowItemByIndex)" << std::endl;
				return -1;
			}

			printCurrentUserPosition(&user, "현재 위치");

			switch(row_ptr->angle_type) {
				case ANGLE_TYPE_UP  :
				case ANGLE_TYPE_DOWN:
				case ANGLE_TYPE_STRAIGHT:
					user.col_pos = row_ptr->opst_cpos;
					user.row_pos = row_ptr->opst_rpos;

					printCurrentUserPosition(&user, "이동...");
				default:
					user.row_pos++;

					printCurrentUserPosition(&user, "한칸 전진.");
					std::cout << std::endl;
				break;
			}
		}

		col_ptr = getColItemByIndex(user.col_pos);
		if(col_ptr == nullptr) {
			std::cerr << "[PLAY] col_ptr is default (getColItemByIndex)" << std::endl;
			return -1;
		}

		user.value = col_ptr->value;
		std::cout << "선택하신 " << position << "번 위치의 결과는 '" << (col_ptr->col_pos + 1) << "번 " << user.value << "'입니다." << std::endl;
	}


	return 0;
}

COLITEM* getPrevColItem(int col_pos)
{
	int prev_pos = col_pos - 1;
	if(prev_pos < 0) {
		return nullptr;
	}

	COLITEM* col_ptr = nullptr;
	try {
		col_ptr = &_global.cols.at(col_pos - 1);
		if(col_ptr->col_pos < col_pos) {
			return col_ptr;
		}

		return nullptr;
	}
	catch(const std::out_of_range& e) {
		std::cerr << "[Prev ][Col] Out of range error: " << e.what() << " [CPOS:" << prev_pos << "]" << std::endl;
		return nullptr;
	}
}

COLITEM* getNextColItem(int col_pos)
{
	int next_pos = col_pos + 1;
	if(next_pos >= _global.cols_cnt) {
		return nullptr;
	}

	COLITEM* col_ptr = nullptr;
	try {
		col_ptr = &_global.cols.at(next_pos);
		if(col_ptr->col_pos > col_pos) {
			return col_ptr;
		}

		return nullptr;
	}
	catch(const std::out_of_range& e) {
		std::cerr << "[Next ][Col] Out of range error: " << e.what() << " [CPOS:" << next_pos << "]" << std::endl;
		return nullptr;
	}
}

COLITEM* getColItemByIndex(int col_pos)
{
	COLITEM* col_ptr = nullptr;
	try {
		col_ptr = &_global.cols.at(col_pos);
		return col_ptr;
	}
	catch(const std::out_of_range& e) {
		std::cerr << "[Index][Col] Out of range error: " << e.what() << " [CPOS:" << col_pos << "]" << std::endl;
		return nullptr;
	}
}

ROWITEM* getPrevRowItem(int col_pos, int row_pos)
{
	int prev_pos = row_pos - 1;
	if(prev_pos < 0) {
		return nullptr;
	}

	COLITEM* col_ptr = getColItemByIndex(col_pos);
	if(col_ptr == nullptr) {
		return nullptr;
	}


	ROWITEM* row_ptr = nullptr;
	try {
		row_ptr = &col_ptr->rows.at(prev_pos);
		if(row_ptr->row_pos < row_pos) {
			return row_ptr;
		}

		return nullptr;
	}
	catch(const std::out_of_range& e) {
		std::cerr << "[Prev ][Row] Out of range error: " << e.what() << " [CPOS:" << col_pos << "][RPOS:" << prev_pos << "]" << std::endl;
		return nullptr;
	}
}

ROWITEM* getNextRowItem(int col_pos, int row_pos)
{
	int next_pos = row_pos + 1;
	if(next_pos >= _global.height) {
		return nullptr;
	}

	COLITEM* col_ptr = getColItemByIndex(col_pos);
	if(col_ptr == nullptr) {
		return nullptr;
	}


	ROWITEM* row_ptr = nullptr;
	try {
		row_ptr = &col_ptr->rows.at(next_pos);
		if(row_ptr->row_pos > row_pos) {
			return row_ptr;
		}

		return nullptr;
	}
	catch(const std::out_of_range& e) {
		std::cerr << "[Next ][Row] Out of range error: " << e.what() << " [CPOS:" << col_pos << "][RPOS:" << next_pos << "]" << std::endl;
		return nullptr;
	}
}

ROWITEM* getRowItemByIndex(int col_pos, int row_pos)
{
	COLITEM* col_ptr = getColItemByIndex(col_pos);
	if(col_ptr == nullptr) {
		return nullptr;
	}

	ROWITEM* row_ptr = nullptr;
	try {
		row_ptr = &col_ptr->rows.at(row_pos);
		return row_ptr;
	}
	catch(const std::out_of_range& e) {
		std::cerr << "[Index][Row] Out of range error: " << e.what() << " [CPOS:" << col_pos << "][RPOS:" << row_pos << "]" << std::endl;
		return nullptr;
	}
}

int getRandomNumber(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(min, max);

	return dis(gen);
}

int getAngleType(int opst_cpos, int row_pos)
{
	int random_num;
	int result;
	int angle_type;

	while(true) {
		random_num = getRandomNumber(0, 99);
		result     = random_num % 3;
		if(result == 0) {
			if(row_pos <= 1) {
				continue;
			}

			angle_type = ANGLE_TYPE_UP;
		}
		else
		if(result == 1) {
			angle_type = ANGLE_TYPE_STRAIGHT;
		}
		else {
			if(row_pos >= (_global.height - 2)) {
				continue;
			}

			angle_type = ANGLE_TYPE_DOWN;
		}

		break;
	}

	return angle_type;
}

int setRowItem(int col_pos, int row_pos, ROWITEM* out_row)
{
	if(col_pos < 0 || col_pos >= _global.cols_cnt) {
		// 발생하면 안되는 에러
		return -1;
	}

	if(row_pos < 0 || row_pos > (_global.height - 1)) {
		// 발생하면 안되는 에러
		return -1;
	}

	out_row->row_pos = row_pos;

	// 처음은 분기점을 생성하지 않고 진행한다
	if(out_row->row_pos == 0) {
		return 0;
	}

	// 마지막은 분기점을 생성하지 않고 종료한다
	if(out_row->row_pos == (_global.height - 1)) {
		return 0;
	}

	int opst_cpos  = 0;
	int opst_rpos  = 0;

	ROWITEM* row_ptr = nullptr;

	// 이전 column에서 분기점이 생성되었다면 그것에 맞춰 세팅한다
	opst_rpos = getRowOppositeByPrevCol(col_pos, out_row->row_pos);
	if(opst_rpos > 0) {
		opst_cpos = col_pos - 1;
		row_ptr   = getRowItemByIndex(opst_cpos, opst_rpos);
		if(row_ptr == nullptr) {
			// 발생하면 안되는 에러
			return -1;
		}

		out_row->used       = 1;
		out_row->opst_cpos  = opst_cpos;
		out_row->opst_rpos  = opst_rpos;
		out_row->angle_type = calcRowAngleTypeByOpposite(out_row->row_pos, out_row->opst_rpos);

		return 1;
	}

	// 마지막은 분기점을 생성하지 않는다
	if(col_pos == (_global.cols_cnt - 1)) {
		return 0;
	}

	int angle_type = 0;
	int used       = getRandomNumber(0, 99) % 2;
	if(used == 0) {
		return 0;
	}

	// 신규생성
	int cnt;
	for(cnt = 0; cnt < 10; cnt++) {
		angle_type = getAngleType(opst_cpos, out_row->row_pos);
		if(angle_type != ANGLE_TYPE_NONE) {
			break;
		}
	}

	// 신규로 생성하기 어려울 것이라고 본다
	if(angle_type == ANGLE_TYPE_NONE) {
		return 0;
	}

	opst_cpos = col_pos + 1;
	opst_rpos = calcRowOppositeByAngleType(out_row->row_pos, angle_type);
	if(opst_rpos < 0) {
		// 발생하면 안되는 에러
		return -1;
	}

	out_row->used       = used;
	out_row->opst_cpos  = opst_cpos;
	out_row->opst_rpos  = opst_rpos;
	out_row->angle_type = angle_type;

	return 1;
}

int _checkRowOppositeByCol(int col_pos, int opst_cpos, int row_pos)
{
	ROWITEM* row = getRowItemByIndex(col_pos, 0);
	while(true) {
		if(row == nullptr) {
			break;
		}

		if(row->opst_cpos == opst_cpos && row->opst_rpos == row_pos) {
			return row->row_pos;
		}

		row = getNextRowItem(col_pos, row->row_pos);
	}

	return 0;
}

int getRowOppositeByPrevCol(int col_pos, int row_pos)
{
	COLITEM* col = getPrevColItem(col_pos);
	if(col != nullptr) {
		return _checkRowOppositeByCol(col->col_pos, col_pos, row_pos);
	}

	return 0;
}

int getRowOppositeByNextCol(int col_pos, int row_pos)
{
	COLITEM* col = getNextColItem(col_pos);
	if(col != nullptr) {
		return _checkRowOppositeByCol(col->col_pos, col_pos, row_pos);
	}

	return 0;
}

int getRowOppositeByPrevNextCol(int col_pos, int row_pos)
{
	int result = getRowOppositeByPrevCol(col_pos, row_pos);
	if(result > 0) {
		return result;
	}

	return getRowOppositeByNextCol(col_pos, row_pos);
}

int calcRowOppositeByAngleType(int row_pos, int angle_type)
{
	switch(angle_type) {
		case ANGLE_TYPE_UP:
			if(row_pos <= 0) {
				return -1;
			}

			return row_pos - 1;
		break;
		case ANGLE_TYPE_STRAIGHT:
			return row_pos;
		break;
		case ANGLE_TYPE_DOWN:
			if(row_pos >= _global.height) {
				return -1;
			}

			return row_pos + 1;
		break;
		default:
			return -1;
	}
}

int calcRowAngleTypeByOpposite(int row_pos, int opst_rpos)
{
	if(row_pos > opst_rpos) {
		return ANGLE_TYPE_UP;
	}
	else
	if(row_pos < opst_rpos) {
		return ANGLE_TYPE_DOWN;
	}
	else {
		return ANGLE_TYPE_STRAIGHT;
	}
}

int inputNumber(std::string description)
{
	int output = 0;
    std::string input;

	if(description.length() > 0) {
		std::cout << description << std::endl;
	}

    std::getline(std::cin, input);
    try {
        int rtn = std::stoi(input.substr(0, input.find(' ')));
        output = static_cast<int>(rtn);
        if(output < 0) {
            output = 0;
        }
    } catch(const std::invalid_argument& e) {
        std::cerr << "0 이상의 수를 입력해주세요." << std::endl;
        return -1;
    }

    return output;
}

// for LOG
int printGhostLegAbout()
{
	COLITEM* col_ptr = nullptr;
	ROWITEM* row_ptr = nullptr;

	col_ptr = getColItemByIndex(0);
	while(true) {
		if(col_ptr == nullptr) {
			break;
		}

		row_ptr = getRowItemByIndex(col_ptr->col_pos, 0);
		while(true) {
			if(row_ptr == nullptr) {
				break;
			}

			std::cout << "=[" << col_ptr->col_pos << "][" << row_ptr->row_pos << "]===================" << std::endl;
			std::cout << " used      : " << row_ptr->used       << std::endl;
			std::cout << " row_pos   : " << row_ptr->row_pos    << std::endl;
			std::cout << " opst_cpos : " << row_ptr->opst_cpos  << std::endl;
			std::cout << " opst_rpos : " << row_ptr->opst_rpos  << std::endl;
			std::cout << " angle_type: " << row_ptr->angle_type << std::endl;
			std::cout << "==========================" << std::endl;

			row_ptr = getNextRowItem(col_ptr->col_pos, row_ptr->row_pos);
		}

		col_ptr = getNextColItem(col_ptr->col_pos);
	}

	return 0;
}

int printCurrentUserPosition(USER* user, std::string description)
{
	std::cout << "[" << (user->col_pos + 1) << "][" << user->row_pos << "]";

	if(description.length() > 0) {
		std::cout << " " << description;
	}

	std::cout << std::endl;

	return 0;
}

