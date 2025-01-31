
#define is_down(b) (input->buttons[b].is_down)
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

// declearing all the variable for making those objects
float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 100, ball_dp_y, ball_half_size = 1;

internal void
stimulate_game(Input* input, float dt) {

	clear_screen(0xff5500); // orange backgound in entire window 
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33); // yellow background for gameplay

	//comtrols moment for player 1
	float player_1_dpp = 0.f;

	if (is_down(BUTTON_UP)) player_1_dpp += 2000;
	if (is_down(BUTTON_DOWN)) player_1_dpp -= 2000;
	
	// controls moment for player 2
	float player_2_dpp = 0.f;
	if (is_down(BUTTON_W)) player_2_dpp += 2000;
	if (is_down(BUTTON_S)) player_2_dpp -= 2000;

	// friction added to the players
	player_1_dpp -= player_1_dp * 10.f;
	player_2_dpp -= player_2_dp * 10.f;
	
	player_1_p = player_1_p + player_1_dp * dt + player_1_dpp * dt * dt * 0.002f; // change 0.005f more or less to change player speed
	player_1_dp = player_1_dp + player_1_dpp * dt;

	//player 1 collosion code
	if (player_1_p + player_half_size_y > arena_half_size_y) {
		player_1_p = arena_half_size_y - player_half_size_y;
		player_1_dp = 0; //make 0 to -50 or more in case you want to bouns back after collision
	}else if (player_1_p - player_half_size_y < -arena_half_size_y) {
		player_1_p = -arena_half_size_y + player_half_size_y;
		player_1_dp = 0; 
	}
	
	player_2_p = player_2_p + player_2_dp * dt + player_2_dpp * dt * dt * 0.005f;
	player_2_dp = player_2_dp + player_2_dpp * dt;

	//player 2 collosion code
	if (player_2_p + player_half_size_y > arena_half_size_y) {
		player_2_p = arena_half_size_y - player_half_size_y;
		player_2_dp = 0; 
	}
	else if (player_2_p - player_half_size_y < -arena_half_size_y) {
		player_2_p = -arena_half_size_y + player_half_size_y;
		player_2_dp = 0; 
	}

	// code for ball moverment
	ball_p_x += ball_dp_x * dt;
	ball_p_y += ball_dp_y * dt;

	draw_rect(ball_p_x, ball_p_y, 1, 1, 0xffffff); // ball graphic

	//ball collision 
	if (ball_p_x + ball_half_size > 80 - player_half_size_x &&
		ball_p_x - ball_half_size < 80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_1_p - player_half_size_y &&
		ball_p_y - ball_half_size < player_1_p + player_half_size_y ) {
			ball_p_x = 80 - player_half_size_x - ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * 0.75f; // changing ball direction after collosion 
	}else if (ball_p_x + ball_half_size > -80 + player_half_size_x &&
		ball_p_x - ball_half_size < -80 + player_half_size_x &&
		ball_p_y + ball_half_size > player_2_p - player_half_size_y &&
		ball_p_y - ball_half_size < player_2_p + player_half_size_y ) {
			ball_p_x =-80 + player_half_size_x + ball_half_size;
			ball_dp_x *= -1;
			ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * 0.75f;
	}

	// limiting ball to stay inside from vertical (up and down) collosion after player hit them
	if (ball_p_y + ball_half_size > arena_half_size_y) {
		ball_p_y = arena_half_size_y - ball_half_size;
		ball_dp_y *= -1;
	}else if (ball_p_y - ball_half_size < -arena_half_size_y) {
		ball_p_y = -arena_half_size_y + ball_half_size;
		ball_dp_y *= -1;
	}

	// bringing ball back to live after each game over
	if (ball_p_x + ball_half_size > arena_half_size_x) {
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_p_x = 0;
		ball_p_y = 0;
	}else if (ball_p_x - ball_half_size < -arena_half_size_x) {
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_p_x = 0;
		ball_p_y = 0;
	}

	draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000); // player 1 graphic
	draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // player 2 graphics

}