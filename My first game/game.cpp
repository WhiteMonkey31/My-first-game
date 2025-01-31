
#define is_down(b) (input->buttons[b].is_down)
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_1_p, player_1_dp, player_2_p, player_2_dp;

internal void
stimulate_game(Input* input, float dt) {

	clear_screen(0xff5500);
	draw_rect(0, 0, 85, 45, 0xffaa33);


	float player_1_dpp = 0.f;

	if (is_down(BUTTON_UP)) player_1_dpp += 2000;
	if (is_down(BUTTON_DOWN)) player_1_dpp -= 2000;
	
	float player_2_dpp = 0.f;
	if (is_down(BUTTON_W)) player_2_dpp += 2000;
	if (is_down(BUTTON_S)) player_2_dpp -= 2000;

	player_1_dpp -= player_1_dp * 10.f;
	player_2_dpp -= player_2_dp * 10.f;
	
	player_1_p = player_1_p + player_1_dp * dt + player_1_dpp * dt * dt * 0.005f;
	player_1_dp = player_1_dp + player_1_dpp * dt;
	
	player_2_p = player_2_p + player_2_dp * dt + player_2_dpp * dt * dt * 0.005f;
	player_2_dp = player_2_dp + player_2_dpp * dt;

	draw_rect(0, 0, 1, 1, 0xffffff);

	draw_rect(80, player_1_p, 2.5, 12, 0xff0000);
	draw_rect(-80, player_2_p, 2.5, 12, 0xff0000);

}