bool jokeSetupDone = false;
bool jokePrinted = false;
typedef struct {
  uint8_t jokeNum;
  const char* setup;
  const char* punchline;
} jokeList;
const jokeList theJokes[] = {{0, "Hey, wanna hear a joke?", "Parsing HTML with regex."}, {1, "Bad at golf?", "Join the club."}, {2, "What do you call sad coffee?", "Despresso."}, {3, "How does a train eat?", "It goes chew, chew"}, {4, "If you boil a clown...", "Do you get a laughing stock?"}, {5, "What does an angry pepper do?", "It gets jalapeño face."}, {6, "When do doctors get angry?", "When they run out of patients."}, {7, "Well...", "That's a deep subject."}, {8, "Where do young cows eat lunch?", "In the calf-ateria."}, {9, "Can February march?", "No, but April may."}, {10, "['hip', 'hip']", "(hip hip array)"}, {11, "Why did the cookie cry?", "It was feeling crumby."}, {12, "Want to hear my pizza joke?", "Never mind, it's too cheesy."}, {13, "Can I watch the TV?", "Yes, but don't turn it on."}, {14, "What's brown and sticky?", "A stick."}, {15, "What goes after USA?", "USB."}, {16, "How do hens stay fit?", "They always egg-cercise!"}, {17, "How much does a hipster weigh?", "An instagram."}, {18, "What do you give a sick lemon?", "Lemonaid."}, {19, "Dad, can you put my shoes on?", "I don't think they'll fit me."}, {20, "What did the 0 say to the 8?", "Nice belt."}, {21, "What has ears but cannot hear?", "A field of corn."}, {22, "How do you make holy water?", "You boil the hell out of it"}, {23, "What kind of bagel can fly?", "A plain bagel."}, {24, "Why are fish so smart?", "Because they live in schools!"}, {25, "Why do bears have hairy coats?", "Fur protection."}, {26, "What's E.T. short for?", "He's only got little legs."}, {27, "Where's the bin?", "I haven't been anywhere!"}, {28, "How do you fix a broken pizza?", "With tomato paste."}, {29, "Why is seven bigger than nine?", "Because seven ate nine."}, {30, "Is the pool safe for diving?", "It deep ends."}, {31, "How do you steal a coat?", "You jacket."}};

void joke_reset() {
  jokeSetupDone = false;
  jokePrinted = false;
}

void joke_drawUI() {
  lcd.setColor(251, 193, 4);
  lcd.drawLine(0, 0, 0, 26);
  lcd.drawLine(0, 0, 480, 0);
  lcd.drawLine(479, 0, 479, 26);
  lcd.drawLine(0, 26, 480, 26);
  lcd.setFont(BigFont);
  lcd.print("Jokes: The Funny App", CENTER, 5);
  lcd.drawLine(0, 299, 480, 299);
  lcd.setFont(SmallFont);
  lcd.print("PRESS BUTTON FOR NEW JOKE!", CENTER, 305);
}

void joke_setup() {
  joke_drawUI();
  jokeSetupDone = true;
}

int splitStringTop(const String& inputString) {
  int yStarting = 40;
  int i = 0;
  while (i < inputString.length()) {
    int spaceIndex = inputString.lastIndexOf(' ', i + 28);
    if (spaceIndex == -1) {
      spaceIndex = i + 28;
    }
    String substring = inputString.substring(i, spaceIndex);
    lcd.print(substring, CENTER, yStarting);
    i += substring.length();
    yStarting += 20;
  }
  return yStarting;
}

int splitStringBottom(const String& inputString) {
  int yStarting = 270;
  int i = inputString.length();
  while (i > 0) {
    int spaceIndex = (i > 28) ? inputString.indexOf(' ', i - 28) : 0;
    if (spaceIndex == -1) {
      spaceIndex = i - 28;
    }
    String substring = inputString.substring(spaceIndex, i);
    lcd.print(substring, CENTER, yStarting);
    i -= substring.length();
    yStarting -= 20;
  }
  return yStarting;
}

void joke_loop() {
  if (!jokeSetupDone) {
    joke_setup();
    lcd.setFont(BigFont);
    lcd.drawBitmap(176, 96, 32, 32, laughingEmoji, 4);
  } else {
    if (!jokePrinted) {
      lcd.setColor(251, 193, 4);
      int randomJoke = random(32);
      int jokeSetup = splitStringTop(theJokes[randomJoke].setup);
      int punchLine = splitStringBottom(theJokes[randomJoke].punchline);
      jokePrinted = true;
    } else {
      if (digitalRead(ButtonH) == LOW) {
        lcd.setColor(0, 0, 0);
        lcd.fillRect(0, 40, 480, 96);
        lcd.fillRect(0, 224, 480, 285);
        jokePrinted = false;
      }
      if (digitalRead(ButtonR) == LOW) {
        joke_reset();
        gameRunning = MENU;
        menu_reset();
      }
    }
  }
}
