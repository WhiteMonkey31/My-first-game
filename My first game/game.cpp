
#define is_down(b) (input->buttons[b].is_down)
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

// declearing all the variable for making those objects
float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_p_x, ball_p_y, ball_dp_x = 130, ball_dp_y, ball_half_size = 1; // ball_dp_x is the ball speed (you can increase or decrease it from here)
int player_1_score, player_2_score;


internal void
stimulate_player(float* p, float* dp, float ddp, float dt) {

	// friction added to the players
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * 0.5f; // change 0.005f more or less to change player speed
	*dp = *dp + ddp * dt;

	//player collosion code
	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0; //make 0 to -50 or more in case you want to bouns back after collision
	}else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return(p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y - hs1y < p2y + hs2y);
}

enum Gamemode {
	GM_MENU,
	GM_GAMEPLAY,
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

internal void
stimulate_game(Input* input, float dt) {

	
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0xffaa33); // yellow background for gameplay

	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0xff5500);

	if (current_gamemode == GM_GAMEPLAY) {
		//comtrols moment for player 1
		float player_1_dpp = 0.f;
		if (enemy_is_ai) {
			if (is_down(BUTTON_UP)) player_1_dpp += 2000;
			if (is_down(BUTTON_DOWN)) player_1_dpp -= 2000;
		}
		else {
			//if (ball_p_y > player_1_p + 2.5f) player_1_dpp += 1300;
			//if (ball_p_y < player_1_p - 2.5f) player_1_dpp -= 1300;
			player_1_dpp = (ball_p_y - player_1_p) * 100;
			if (player_1_dpp > 1300) player_1_dpp = 1300;
			if (player_1_dpp < -1300) player_1_dpp = -1300;
		}
		// controls moment for player 2
		float player_2_dpp = 0.f;
		if (is_down(BUTTON_W)) player_2_dpp += 2000;
		if (is_down(BUTTON_S)) player_2_dpp -= 2000;

		stimulate_player(&player_1_p, &player_1_dp, player_1_dpp, dt);
		stimulate_player(&player_2_p, &player_2_dp, player_2_dpp, dt);

		// stimulate ball
		{
			// code for ball moverment
			ball_p_x += ball_dp_x * dt;
			ball_p_y += ball_dp_y * dt;


			//ball collision 
			if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 80, player_1_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = 80 - player_half_size_x - ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_1_p) * 2 + player_1_dp * 0.75f; // changing ball direction after collosion 
			}
			else if (aabb_vs_aabb(ball_p_x, ball_p_y, ball_half_size, ball_half_size, -80, player_2_p, player_half_size_x, player_half_size_y)) {
				ball_p_x = -80 + player_half_size_x + ball_half_size;
				ball_dp_x *= -1;
				ball_dp_y = (ball_p_y - player_2_p) * 2 + player_2_dp * 0.75f;
			}

			// limiting ball to stay inside from vertical (up and down) collosion after player hit them
			if (ball_p_y + ball_half_size > arena_half_size_y) {
				ball_p_y = arena_half_size_y - ball_half_size;
				ball_dp_y *= -1;
			}
			else if (ball_p_y - ball_half_size < -arena_half_size_y) {
				ball_p_y = -arena_half_size_y + ball_half_size;
				ball_dp_y *= -1;
			}

			// bringing ball back to live after each game over
			if (ball_p_x + ball_half_size > arena_half_size_x) {
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_1_score++;
			}
			else if (ball_p_x - ball_half_size < -arena_half_size_x) {
				ball_dp_x *= -1;
				ball_dp_y = 0;
				ball_p_x = 0;
				ball_p_y = 0;
				player_2_score++;
			}
		}

		// showing player score by box on both sides top
		draw_number(player_1_score, -10, 40, 1.f, 0xbbffbb);
		draw_number(player_2_score, 10, 40, 1.f, 0xbbffbb);

		//renering
		draw_rect(ball_p_x, ball_p_y, ball_half_size, ball_half_size, 0xffffff); // ball graphic
		draw_rect(80, player_1_p, player_half_size_x, player_half_size_y, 0xff0000); // player 1 graphic
		draw_rect(-80, player_2_p, player_half_size_x, player_half_size_y, 0xff0000); // player 2 graphics
	}
	else { // game menu
		if (pressed(BUTTON_LEFT) || pressed(BUTTON_RIGHT)) {
			hot_button = !hot_button;
		}
		if (pressed(BUTTON_ENTER)) {
			current_gamemode = GM_GAMEPLAY;
			enemy_is_ai = hot_button ? 0 : 1;
		}
		if (hot_button == 0) {
			draw_text("SINGLE PLAYER", -85, -10, 1, 0xff0000);
			draw_text("MULTI PLAYER", 15, -10, 1, 0xcccccc);
		}
		else {
			draw_text("SINGLE PLAYER", -85, -10, 1, 0xcccccc);
			draw_text("MULTI PLAYER", 15, -10, 1, 0xff0000);
		}
		
		draw_text("PONG GAME", -73, 40, 2, 0xffffff);
		draw_text("CREATED BY SAIF UR REHMAN AWAN", -73, 22, 0.75, 0xffffff);
		draw_text("MY FIRST GAME", -73, 15, 1.22, 0xffffff);

	}
}