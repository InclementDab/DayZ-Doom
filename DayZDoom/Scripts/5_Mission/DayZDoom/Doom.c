typedef array<ref Widget> PixelArray;

class PixelGameEngine
{	
	static const int SCREEN_WIDTH = 50;
	static const int SCREEN_HEIGHT = 28;
	static const int PIXEL_SIZE = 10;
	static const int FRAMERATE = 20;
	
	protected Widget m_LayoutRoot;
	protected ref array<ref PixelArray> m_ScreenPixels = {};
	protected ref Timer m_FrameTimer = new Timer(CALL_CATEGORY_SYSTEM);
	
	void PixelGameEngine()
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
		
		m_FrameTimer.Run(1 / FRAMERATE, this, "_OnFrame", new Param1<float>(GetGame().GetTime()), true);
	}
	
	void ~PixelGameEngine()
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
	
	private void _OnFrame(float start_time)
	{
		for (int i = 0; i < SCREEN_WIDTH; i++) {
			for (int j = 0; j < SCREEN_HEIGHT; j++) {
				Draw(i, j, ARGB(255, 0, 0, 0));	
			}
		}
		
		float tfactor = (GetGame().GetTime() - start_time) / 1000;
		OnFrame(tfactor);
	}
	
	void OnFrame(float dt);
	
	
	void Draw(int pixel_x, int pixel_y, int color)
	{
		if (pixel_x < 0 || pixel_x >= SCREEN_WIDTH || pixel_y < 0 || pixel_y >= SCREEN_HEIGHT) {
			return;
		}
		
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
	
	
	void DrawCircle(int xc, int yc, int r, int c)
	{
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		while (y >= x) // only formulate 1/8 of circle
		{
			Draw(xc - x, yc - y, c);//upper left left
			Draw(xc - y, yc - x, c);//upper upper left
			Draw(xc + y, yc - x, c);//upper upper right
			Draw(xc + x, yc - y, c);//upper right right
			Draw(xc - x, yc + y, c);//lower left left
			Draw(xc - y, yc + x, c);//lower lower left
			Draw(xc + y, yc + x, c);//lower lower right
			Draw(xc + x, yc + y, c);//lower right right
			if (p < 0) { 
				x++;
				p += 4 * x + 6;
			}
			else {
				x++;
				y--;
				p += 4 * (x - y) + 10;
			}
		}
	}
	
	private void _drawline(int sx, int ex, int ny, int c)
	{
		for (int i = sx; i <= ex; i++)
			Draw(i, ny, c);
	}
	
	void FillCircle(int xc, int yc, int r, int c)
	{
		// Taken from wikipedia
		int x = 0;
		int y = r;
		int p = 3 - 2 * r;
		if (!r) return;

		while (y >= x) {
			// Modified to draw scan-lines instead of edges
			_drawline(xc - x, xc + x, yc - y, c);
			_drawline(xc - y, xc + y, yc - x, c);
			_drawline(xc - x, xc + x, yc + y, c);
			_drawline(xc - y, xc + y, yc + x, c);
			if (p < 0) { 
				x++;
				p += 4 * x + 6;
			}
			else {
				x++;
				y--;
				p += 4 * (x - y) + 10;
			}
		}
	}
	
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int c)
	{
		DrawLine(x1, y1, x2, y2, c);
		DrawLine(x2, y2, x3, y3, c);
		DrawLine(x3, y3, x1, y1, c);
	}

}