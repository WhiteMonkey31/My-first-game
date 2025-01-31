
#define is_down(b) (input->buttons[b].is_down)
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_p = 0.f;

internal void
stimulate_game(Input* input, float dt) {

	clear_screen(0xff5500);
	draw_rect(0, 0, 85, 45, 0xffaa33);


	float speed = 50.f;

	if (is_down(BUTTON_UP)) player_p += speed*dt;
	if (is_down(BUTTON_DOWN)) player_p -= speed*dt;
	

	draw_rect(0, 0, 1, 1, 0xffffff);

	draw_rect(80, player_p, 2.5, 12, 0xff0000);
	draw_rect(-80, 0, 2.5, 12, 0xff0000);

}