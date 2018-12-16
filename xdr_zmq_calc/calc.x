
const ADD       = 601;
const SUBTRACT  = 602;
const MULTIPLY  = 603;
const DIVIDE    = 604;
const FACTORIAL = 605;

struct AddWork {
  int num1;
  int num2;
  int num3;
  int num4;
};

struct AddResult {
  int num;
  int err;
};

struct SubWork {
  int num1;
  int num2;
};

struct SubResult {
  int num;
  int err;
};

struct MulWork {
  int num1;
  int num2;
  int num3;
};

struct MulResult {
  int num;
  int err;
};

struct DivWork {
  int num1;
  int num2;
};

struct DivResult {
  int num;
  int err;
};

struct FacWork {
  int num1;
};

struct FacResult {
  int num;
  int err;
};

union calc_req switch ( int op ) {
    case ADD       : AddWork add_request;
    case SUBTRACT  : SubWork sub_request;
    case MULTIPLY  : MulWork mul_request;
    case DIVIDE    : DivWork div_request;
    case FACTORIAL : FacWork fac_request;
};

union calc_res switch ( int op ) {
    case ADD       : AddResult add_result;
    case SUBTRACT  : SubResult sub_result;
    case MULTIPLY  : MulResult mul_result;
    case DIVIDE    : DivResult div_result;
    case FACTORIAL : FacWork fac_result;
};
