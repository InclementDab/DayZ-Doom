const int COLOR_PONG = ARGB(255, 255, 255, 255);

class SpriteBase
{
	void Draw(PixelGameEngine game_engine);
}

class Line: SpriteBase
{
	protected int X1, X2, Y1, Y2, Color;
	
	void Line(int x1, int y1, int x2, int y2, int color = COLOR_PONG)
	{
		X1 = x1; X2 = x2; Y1 = y1; Y2 = y2;
	}
	
	void SetPosition(int x1, int y1, int x2, int y2)
	{
		X1 = x1; X2 = x2; Y1 = y1; Y2 = y2;
	}
	
	void SetColor(int color)
	{
		Color = color;
	}
	
	override void Draw(PixelGameEngine game_engine)
	{
		game_engine.DrawLine(X1, Y1, X2, Y2, Color);
	}
}

class Circle: SpriteBase
{
	
}

class Pong: PixelGameEngine
{
	// static constructor style
	// cause i really want to type Doom.Run(); lol
	private static ref Pong m_Instance;
	
	static void Run()
	{
		/*if (GetGame().IsServer()) {
			return;
		}*/
		
		if (m_Instance) {
			delete m_Instance;
		}
		
		typename _typ = Pong;
		m_Instance = Pong.Cast(_typ.Spawn());	
	}
	
	static void Stop()
	{
		delete m_Instance;
	}
	
	static Pong GetInstance()
	{
		return m_Instance;
	}
	
	const int PADDLE_SIZE = 8;
	const float PADDLE_SPEED = 0.05;
	const float BALL_SPEED = 0.1;
	const int BALL_DIAMETER = 1;
	const int PADDLE_OFFSET = 1;
	const float MAX_BOUNCE_ANGLE = 45;
	
	protected int player_1_pos = 0;
	protected int player_2_pos = 0;

	
	protected float ball_x, ball_y, ball_vx, ball_vy;
	
	private void Pong()
	{
		player_1_pos = SCREEN_HEIGHT / 2;
		player_2_pos = SCREEN_HEIGHT / 2;
		
		ball_x = SCREEN_WIDTH / 2;
		ball_y = SCREEN_HEIGHT / 2;
		
		thread StartGame();
	}
	
	private void StartGame()
	{
		Sleep(2000);
		ball_vx = -0.1;
	}
	
	override void OnFrame(float dt)
	{		
		Input input = GetGame().GetInput();
		
		if (input.LocalValue("UAMoveForward")) {
			SetPlayer1Position(player_1_pos - 1);
		}
		
		if (input.LocalValue("UAMoveBack")) {
			SetPlayer1Position(player_1_pos + 1);
		}
		
		if (input.LocalValue("UAUIUp")) {
			SetPlayer2Position(player_2_pos - 1);
		}
		
		if (input.LocalValue("UAUIDown")) {
			SetPlayer2Position(player_2_pos + 1);
		}	
		
		if (input.LocalValue("UAUILeft")) {
			ball_vx -= 0.01;
		}
		
		if (input.LocalValue("UAUIRight")) {
			ball_vx += 0.01;
		}
		
		// player 1
		DrawLine(0, player_1_pos + (PADDLE_SIZE / 2), 0, player_1_pos - (PADDLE_SIZE / 2), COLOR_PONG);
		
		// player 2
		DrawLine(SCREEN_WIDTH - 2, player_2_pos + (PADDLE_SIZE / 2), SCREEN_WIDTH - 2, player_2_pos - (PADDLE_SIZE / 2), COLOR_PONG);
		
		// ball
		/*if (ball_x <= 3 && (ball_y <= player_1_pos + (PADDLE_SIZE / 2) && ball_y >= player_1_pos - (PADDLE_SIZE / 2))) {
			ball_vx = 0.03;
		}
		
		if (ball_x >= SCREEN_WIDTH - 3 && (ball_y <= player_2_pos + (PADDLE_SIZE / 2) && ball_y >= player_2_pos - (PADDLE_SIZE / 2))) {
			ball_vx = -0.03;
		}
		*/
		float newBallX = ball_x + ball_vx * dt;
        float newBallY = ball_y + ball_vy * dt;

	    // Top and bottom edges, simply bounce
	    if (newBallY < 0) {
	        newBallY = -newBallY;
	        ball_vy = -ball_vy;
	    } else if (newBallY + BALL_DIAMETER > (SCREEN_HEIGHT - 1)) {
	        newBallY -= 2 * ((newBallY + BALL_DIAMETER) - (SCREEN_HEIGHT - 1));
	        ball_vy = -ball_vy;
	    }
	
	    float intersectX, intersectY, relativeIntersectY, bounceAngle, ballSpeed, ballTravelLeft;
	
	    // Left paddle (paddle1)
	 	if (ball_x <= 3 && (ball_y <= player_1_pos + (PADDLE_SIZE / 2) && ball_y >= player_1_pos - (PADDLE_SIZE / 2))) {
	        intersectX = PADDLE_OFFSET; // (duh)
	        intersectY = ball_y - ((ball_x - PADDLE_OFFSET) * (ball_y - newBallY)) / (ball_x - newBallX);
	        if (intersectY >= player_1_pos && intersectY <= player_1_pos + PADDLE_SIZE) {
	            relativeIntersectY = (player_1_pos + (PADDLE_SIZE / 2)) - intersectY;
	            bounceAngle = (relativeIntersectY / (PADDLE_SIZE / 2)) * (Math.PI / 2 - MAX_BOUNCE_ANGLE);
	            ballSpeed = Math.Sqrt(ball_vx * ball_vx + ball_vy * ball_vy);
	            ballTravelLeft = (newBallY - intersectY) / (newBallY - ball_y);
	            ball_vx = ballSpeed * Math.Cos(bounceAngle);
	            ball_vy = ballSpeed * -Math.Sin(bounceAngle);
	            newBallX = intersectX + ballTravelLeft * ballSpeed * Math.Cos(bounceAngle);
	            newBallY = intersectY + ballTravelLeft * ballSpeed * Math.Sin(bounceAngle);
	        }
	    }
	
	    // Right paddle (paddle2)
	    if (ball_x >= SCREEN_WIDTH - 3 && (ball_y <= player_2_pos + (PADDLE_SIZE / 2) && ball_y >= player_2_pos - (PADDLE_SIZE / 2))) {
	        intersectX = SCREEN_WIDTH - PADDLE_OFFSET; // (duh)
	        intersectY = ball_y - ((ball_x - (SCREEN_WIDTH - PADDLE_OFFSET)) * (ball_y - newBallY)) / (ball_x - newBallX);
	        if (intersectY >= player_2_pos && intersectY <= player_2_pos + PADDLE_SIZE) {
	            relativeIntersectY = (player_2_pos + (PADDLE_SIZE / 2)) - intersectY;
	            bounceAngle = (relativeIntersectY / (PADDLE_SIZE / 2)) * (Math.PI / 2 - MAX_BOUNCE_ANGLE);
	            ballSpeed = Math.Sqrt(ball_vx * ball_vx + ball_vy * ball_vy);
	            ballTravelLeft = (newBallY - intersectY) / (newBallY - ball_y);
	            ball_vx = ballSpeed * Math.Cos(bounceAngle) * -1;
	            ball_vy = ballSpeed * Math.Sin(bounceAngle) * -1;
	            newBallX = intersectX - ballTravelLeft * ballSpeed * Math.Cos(bounceAngle);
	            newBallY = intersectY - ballTravelLeft * ballSpeed * Math.Sin(bounceAngle);
	        }
	    }
		
		
		//ball_x += ball_vx * dt;
		//ball_y += ball_vy * dt;
		ball_x = newBallX;
		ball_y = newBallY;
		Draw(ball_x, ball_y, COLOR_PONG);
	}
	
	void SetPlayer1Position(int position)
	{
		if (player_1_pos - (PADDLE_SIZE / 2) + position <= 0) {
			 position = (PADDLE_SIZE / 2);
		}
		
		if (player_1_pos + (PADDLE_SIZE / 2) > SCREEN_HEIGHT) {
			position = SCREEN_HEIGHT - (PADDLE_SIZE / 2);
		}
		
		player_1_pos = position;
	}
	
	void SetPlayer2Position(int position)
	{
		if (player_2_pos - (PADDLE_SIZE / 2) + position <= 0) {
			 position = (PADDLE_SIZE / 2);
		}
		
		if (player_2_pos + (PADDLE_SIZE / 2) > SCREEN_HEIGHT) {
			position = SCREEN_HEIGHT - (PADDLE_SIZE / 2);
		}
		
		player_2_pos = position;
	}
}