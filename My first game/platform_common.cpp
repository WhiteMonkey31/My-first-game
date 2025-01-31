struct Button_State {
	bool is_down,
		changed;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_LEFT,

	BUTTON_COUNT, //Should be the last item
};

struct Input {
	Button_State buttons[BUTTON_COUNT];
};