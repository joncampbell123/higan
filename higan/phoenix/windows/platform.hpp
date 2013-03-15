namespace phoenix {

struct pApplication {
  static void run();
  static bool pendingEvents();
  static void processEvents();
  static void quit();

  static void initialize();
};

struct Settings {
  bidirectional_map<Keyboard::Scancode, unsigned> keymap;
};

struct pFont;
struct pObject;
struct pWindow;
struct pMenu;
struct pLayout;
struct pWidget;

struct pFont {
  static string serif(unsigned size, string style);
  static string sans(unsigned size, string style);
  static string monospace(unsigned size, string style);
  static Size size(const string &font, const string &text);

  static HFONT create(const string &description);
  static void free(HFONT hfont);
  static Size size(HFONT hfont, const string &text);
};

struct pDesktop {
  static Size size();
  static Geometry workspace();
};

struct pKeyboard {
  static bool pressed(Keyboard::Scancode scancode);
  static vector<bool> state();

  static void initialize();
};

struct pMouse {
  static Position position();
  static bool pressed(Mouse::Button button);
};

struct pDialogWindow {
  static string fileOpen(Window &parent, const string &path, const lstring &filter);
  static string fileSave(Window &parent, const string &path, const lstring &filter);
  static string folderSelect(Window &parent, const string &path);
};

struct pMessageWindow {
  static MessageWindow::Response information(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response question(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response warning(Window &parent, const string &text, MessageWindow::Buttons buttons);
  static MessageWindow::Response critical(Window &parent, const string &text, MessageWindow::Buttons buttons);
};

struct pObject {
  static vector<pObject*> objects;

  Object &object;
  uintptr_t id;
  bool locked;

  pObject(Object &object);
  static pObject* find(unsigned id);
  virtual ~pObject() {}

  void constructor() {}
  void destructor() {}
};

struct pTimer : public pObject {
  Timer &timer;
  UINT_PTR htimer;

  void setEnabled(bool enabled);
  void setInterval(unsigned milliseconds);

  pTimer(Timer &timer) : pObject(timer), timer(timer) {}
  void constructor();
};

struct pWindow : public pObject {
  static vector<pWindow*> modal;
  static void updateModality();

  Window &window;
  HWND hwnd;
  HMENU hmenu;
  HWND hstatus;
  HFONT hstatusfont;
  HBRUSH brush;
  COLORREF brushColor;

  static Window& none();

  void append(Layout &layout);
  void append(Menu &menu);
  void append(Widget &widget);
  Color backgroundColor();
  bool focused();
  Geometry frameMargin();
  Geometry geometry();
  void remove(Layout &layout);
  void remove(Menu &menu);
  void remove(Widget &widget);
  void setBackgroundColor(const Color &color);
  void setFocused();
  void setFullScreen(bool fullScreen);
  void setGeometry(const Geometry &geometry);
  void setMenuFont(const string &font);
  void setMenuVisible(bool visible);
  void setModal(bool modal);
  void setResizable(bool resizable);
  void setStatusFont(const string &font);
  void setStatusText(const string &text);
  void setStatusVisible(bool visible);
  void setTitle(const string &text);
  void setVisible(bool visible);
  void setWidgetFont(const string &font);

  pWindow(Window &window) : pObject(window), window(window) {}
  void constructor();
  void destructor();
  void updateMenu();
};

struct pAction : public pObject {
  Action &action;
  Menu *parentMenu;
  Window *parentWindow;

  void setEnabled(bool enabled);
  void setVisible(bool visible);

  pAction(Action &action) : pObject(action), action(action) {}
  void constructor();
};

struct pMenu : public pAction {
  Menu &menu;
  HMENU hmenu;
  HBITMAP hbitmap;

  void append(Action &action);
  void remove(Action &action);
  void setImage(const image &image);
  void setText(const string &text);

  pMenu(Menu &menu) : pAction(menu), menu(menu), hbitmap(0) {}
  void constructor();
  void destructor();
  void createBitmap();
  void update(Window &parentWindow, Menu *parentMenu = 0);
};

struct pSeparator : public pAction {
  Separator &separator;

  pSeparator(Separator &separator) : pAction(separator), separator(separator) {}
  void constructor();
  void destructor();
};

struct pItem : public pAction {
  Item &item;
  HBITMAP hbitmap;

  void setImage(const image &image);
  void setText(const string &text);

  pItem(Item &item) : pAction(item), item(item), hbitmap(0) {}
  void constructor();
  void destructor();
  void createBitmap();
};

struct pCheckItem : public pAction {
  CheckItem &checkItem;

  bool checked();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckItem(CheckItem &checkItem) : pAction(checkItem), checkItem(checkItem) {}
  void constructor();
  void destructor();
};

struct pRadioItem : public pAction {
  RadioItem &radioItem;

  bool checked();
  void setChecked();
  void setGroup(const set<RadioItem&> &group);
  void setText(const string &text);

  pRadioItem(RadioItem &radioItem) : pAction(radioItem), radioItem(radioItem) {}
  void constructor();
  void destructor();
};

struct pSizable : public pObject {
  Sizable &sizable;

  pSizable(Sizable &sizable) : pObject(sizable), sizable(sizable) {}
};

struct pLayout : public pSizable {
  Layout &layout;

  pLayout(Layout &layout) : pSizable(layout), layout(layout) {}
};

struct pWidget : public pSizable {
  Widget &widget;
  Window *parentWindow;
  HWND hwnd;
  HFONT hfont;

  bool enabled();
  bool focused();
  virtual Size minimumSize();
  void setEnabled(bool enabled);
  void setFocused();
  void setFont(const string &font);
  virtual void setGeometry(const Geometry &geometry);
  void setVisible(bool visible);

  pWidget(Widget &widget) : pSizable(widget), widget(widget) { parentWindow = &Window::none(); }
  void constructor();
  void destructor();
  virtual void orphan();
  void setDefaultFont();
  void synchronize();
};

struct pButton : public pWidget {
  Button &button;
  HBITMAP hbitmap;
  HIMAGELIST himagelist;

  Size minimumSize();
  void setImage(const image &image, Orientation orientation);
  void setText(const string &text);

  pButton(Button &button) : pWidget(button), button(button), hbitmap(0), himagelist(0) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pCanvas : public pWidget {
  Canvas &canvas;
  uint32_t *data;

  void setSize(const Size &size);
  void update();

  pCanvas(Canvas &canvas) : pWidget(canvas), canvas(canvas) {}
  void constructor();
  void destructor();
  void orphan();
  void paint();
};

struct pCheckButton : public pWidget {
  CheckButton &checkButton;

  bool checked();
  Size minimumSize();
  void setChecked(bool checked);
  void setText(const string &text);

  pCheckButton(CheckButton &checkButton) : pWidget(checkButton), checkButton(checkButton) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pComboButton : public pWidget {
  ComboButton &comboButton;

  void append(const string &text);
  void modify(unsigned row, const string &text);
  void remove(unsigned row);
  Size minimumSize();
  void reset();
  unsigned selection();
  void setSelection(unsigned row);

  pComboButton(ComboButton &comboButton) : pWidget(comboButton), comboButton(comboButton) {}
  void constructor();
  void destructor();
  void orphan();
  void setGeometry(const Geometry &geometry);
};

struct pHexEdit : public pWidget {
  HexEdit &hexEdit;
  LRESULT CALLBACK (*windowProc)(HWND, UINT, LPARAM, WPARAM);

  void setColumns(unsigned columns);
  void setLength(unsigned length);
  void setOffset(unsigned offset);
  void setRows(unsigned rows);
  void update();

  pHexEdit(HexEdit &hexEdit) : pWidget(hexEdit), hexEdit(hexEdit) {}
  void constructor();
  void destructor();
  void orphan();
  bool keyPress(unsigned key);
};

struct pHorizontalScroller : public pWidget {
  HorizontalScroller &horizontalScroller;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalScroller(HorizontalScroller &horizontalScroller) : pWidget(horizontalScroller), horizontalScroller(horizontalScroller) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pHorizontalSlider : public pWidget {
  HorizontalSlider &horizontalSlider;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pHorizontalSlider(HorizontalSlider &horizontalSlider) : pWidget(horizontalSlider), horizontalSlider(horizontalSlider) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pLabel : public pWidget {
  Label &label;

  Size minimumSize();
  void setText(const string &text);

  pLabel(Label &label) : pWidget(label), label(label) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pLineEdit : public pWidget {
  LineEdit &lineEdit;

  Size minimumSize();
  void setEditable(bool editable);
  void setText(const string &text);
  string text();

  pLineEdit(LineEdit &lineEdit) : pWidget(lineEdit), lineEdit(lineEdit) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pListView : public pWidget {
  ListView &listView;
  HIMAGELIST imageList;
  vector<vector<unsigned>> imageMap;
  vector<image> images;
  bool lostFocus;

  void append(const lstring &text);
  void autoSizeColumns();
  bool checked(unsigned row);
  void modify(unsigned row, const lstring &text);
  void remove(unsigned row);
  void reset();
  bool selected();
  unsigned selection();
  void setCheckable(bool checkable);
  void setChecked(unsigned row, bool checked);
  void setHeaderText(const lstring &text);
  void setHeaderVisible(bool visible);
  void setImage(unsigned row, unsigned column, const image &image);
  void setSelected(bool selected);
  void setSelection(unsigned row);

  pListView(ListView &listView) : pWidget(listView), listView(listView), imageList(nullptr) {}
  void constructor();
  void destructor();
  void orphan();
  void setGeometry(const Geometry &geometry);
  void buildImageList();
};

struct pProgressBar : public pWidget {
  ProgressBar &progressBar;

  Size minimumSize();
  void setPosition(unsigned position);

  pProgressBar(ProgressBar &progressBar) : pWidget(progressBar), progressBar(progressBar) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pRadioButton : public pWidget {
  RadioButton &radioButton;

  bool checked();
  Size minimumSize();
  void setChecked();
  void setGroup(const set<RadioButton&> &group);
  void setText(const string &text);

  pRadioButton(RadioButton &radioButton) : pWidget(radioButton), radioButton(radioButton) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pTextEdit : public pWidget {
  TextEdit &textEdit;

  void setCursorPosition(unsigned position);
  void setEditable(bool editable);
  void setText(const string &text);
  void setWordWrap(bool wordWrap);
  string text();

  pTextEdit(TextEdit &textEdit) : pWidget(textEdit), textEdit(textEdit) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pVerticalScroller : public pWidget {
  VerticalScroller &verticalScroller;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalScroller(VerticalScroller &verticalScroller) : pWidget(verticalScroller), verticalScroller(verticalScroller) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pVerticalSlider : public pWidget {
  VerticalSlider &verticalSlider;

  Size minimumSize();
  unsigned position();
  void setLength(unsigned length);
  void setPosition(unsigned position);

  pVerticalSlider(VerticalSlider &verticalSlider) : pWidget(verticalSlider), verticalSlider(verticalSlider) {}
  void constructor();
  void destructor();
  void orphan();
};

struct pViewport : public pWidget {
  Viewport &viewport;

  uintptr_t handle();

  pViewport(Viewport &viewport) : pWidget(viewport), viewport(viewport) {}
  void constructor();
  void destructor();
  void orphan();
};

}
