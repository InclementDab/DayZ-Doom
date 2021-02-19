typedef array<ref Widget> PixelArray;

class Doom
{
	// static constructor style
	// cause i really want to type Doom.Run(); lol
	private static ref Doom m_Instance;
	
	static void Run()
	{
		/*if (GetGame().IsServer()) {
			return;
		}*/
		
		if (m_Instance) {
			delete m_Instance;
		}
		
		m_Instance = new Doom();	
	}
	
	static void Stop()
	{
		delete m_Instance;
	}
	
	static Doom GetInstance()
	{
		return m_Instance;
	}
	
	static const int SCREEN_WIDTH = 50;
	static const int SCREEN_HEIGHT = 28;
	static const int PIXEL_SIZE = 10;
	static const int FRAMERATE = 20;
	
	protected Widget m_LayoutRoot;
	protected ref array<ref PixelArray> m_ScreenPixels = {};
	protected ref Timer m_FrameTimer = new Timer(CALL_CATEGORY_SYSTEM);
	
	private void Doom()
	{
		m_LayoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZDoom/GUI/layouts/panel_display.layout");
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			if (!m_ScreenPixels[i]) {
				m_ScreenPixels.InsertAt(new PixelArray(), i);
			}
			
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				//Widget pixel = GetGame().GetWorkspace().CreateWidget(PanelWidgetTypeID, i * PIXEL_SIZE, j * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE | WidgetFlags.VISIBLE | WidgetFlags.IGNOREPOINTER, COLOR_YELLOW, 0, m_LayoutRoot);
				Widget pixel = GetGame().GetWorkspace().CreateWidgets("DayZDoom/GUI/layouts/panel_pixel.layout", m_LayoutRoot);
				pixel.SetPos(i * PIXEL_SIZE, j * PIXEL_SIZE);
				pixel.SetColor(COLOR_RED);
				m_ScreenPixels[i].InsertAt(pixel, j);
			}
		}
		
		m_FrameTimer.Run(1 / FRAMERATE, this, "OnFrame", null, true);
	}
	
	void ~Doom()
	{
		m_FrameTimer.Stop();
		delete m_FrameTimer;
		
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				if (m_ScreenPixels[i][j]) {
					m_ScreenPixels[i][j].Unlink();
					delete m_ScreenPixels[i][j];
				}
			}
		}
		
		delete m_ScreenPixels;
		
		if (m_LayoutRoot) {
			m_LayoutRoot.Unlink();
			delete m_LayoutRoot;	
		}
	}
	
	void OnFrame()
	{
		vector dir = GetGame().GetCurrentCameraDirection();		
		
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				Draw(i, j, ARGB(255, 255, 255, 255));	
			}
		}
		float dt = (dir[2] + 1) / 2;
		
		Print(dt);
		DrawLine(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH * dt, SCREEN_HEIGHT * dt, COLOR_RED);
	}
	
	void Draw(int pixel_x, int pixel_y, int color)
	{
		m_ScreenPixels[pixel_x][pixel_y].SetColor(color);
	}
	
	// I stole this code, unapologetically
	void DrawLine(int x1, int y1, int x2, int y2, int c)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = Math.AbsInt(dx); dy1 = Math.AbsInt(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
				{ x = x1; y = y1; xe = x2; }
			else
				{ x = x2; y = y2; xe = x1;}

			Draw(x, y, c);
			
			for (i = 0; x<xe; i++)
			{
				x = x + 1;
				if (px<0)
					px = px + 2 * dy1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, c);
			}
		}
		else
		{
			if (dy >= 0)
				{ x = x1; y = y1; ye = y2; }
			else
				{ x = x2; y = y2; ye = y1; }

			Draw(x, y, c);

			for (i = 0; y<ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, c);
			}
		}
	}
}