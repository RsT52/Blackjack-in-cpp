#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>


using namespace std;

//Создание класса карт
class Card
{
    int power;
    char suit;
public:
    Card() : power(0), suit(' ') {}

    Card(int a, char b)
    {
        power = a;
        suit = b;
    }

    int getPower() const
    {
        if (power >= 11 && power <= 13)
        {
            return 10;
        }
        else if (power == 14)
        {
            return 11;
        }
        else
        {
            return power;
        }

    }

    char getSuit() const
    {
        return suit;
    }
};

vector<Card> createDeck()
{
    vector <Card> deck;
    char suits[4] = { 'p','b','t','h' };


    for (char s : suits)
    {
        for (int i = 2; i < 15; i++)
        {
            if (i != 5)
            {
                deck.push_back(Card(i, s));
            }
        }
    }
    return deck;
}


void shuffleDeck(vector<Card>& deck)
{
    srand(time(0));

    for (int i = 0; i < 256; i++) {
        int a = rand() % deck.size();
        int b = rand() % deck.size();
        if (a != b) {
            std::swap(deck[a], deck[b]);
        }
    }

}

class Player
{
    string name;
    vector<Card*> hand;

public:
    Player() {}
    void getCardInhand(Card* card)
    {
        hand.push_back(card);
    }

    int getSummCardInHand(bool isDraw)
    {
        int summ = 0;
        bool hasAce = false;

        for (size_t i = 0; i < hand.size(); i++)
        {
            if (i == 1 && !isDraw)
            {
                continue;
            }

            summ = summ + hand.at(i)->getPower();

            
            if (hand.at(i)->getPower() == 11)
            {
                hasAce = true;
            }
        }

        
        if (summ > 21 && hasAce)
        {
            summ = summ - 10;
        }

        return summ;
    }

    int getCountCardInhand()
    {
        return hand.size();
    }

    vector<Card*>& getHand()
    {
        return hand;
    }


    bool hasBlackjack() {
        return getSummCardInHand(true) == 21;
    }


};


void drawPlayerHand(Player* player, bool isDraw)
{
    for (size_t i = 0; i < player->getHand().size(); i++)
    {
        if (i == 1 && !isDraw)
        {
            cout << "|------|";
        }
        else
        {
            cout << "| ";
            string power = "";
            int cardPower = player->getHand().at(i)->getPower();
            int randomCard = rand() % 3;
            switch (cardPower)
            {
            case 10:
                if (randomCard == 0) {
                    power = "10";
                }
                else if (randomCard == 1)
                {
                    power = "Валет";
                }
                else if (randomCard == 2)
                {
                    power = "Дама";
                }
                else
                {
                    power = "Король";
                }
                break;
            case 11:
                power = "Туз";
                break;
            default:
                power = to_string(player->getHand().at(i)->getPower());
                break;
            }
            cout << power;

            char suit = player->getHand().at(i)->getSuit();
            if (suit == 'p')
            {
                cout << " Пики ";
            }
            else if (suit == 'b')
            {
                cout << " Бубны ";
            }
            else if (suit == 't')
            {
                cout << " Трефы ";
            }
            else
            {
                cout << " Черви ";
            }
            cout << "|";
        }
    }
}



Card getRandomUniqueCard(vector<Card>& deck)
{
    shuffleDeck(deck);

    // Получаем первую карту из перемешанной колоды
    Card card = deck.back();

    // Удаляем эту карту из колоды, чтобы она больше не повторялась
    deck.pop_back();

    return card;
}


int main()
{
    vector<Card> deck = createDeck();
    vector<Card> initialDeck = deck;
    shuffleDeck(deck);
    setlocale(LC_ALL, "ru");


    int choice;
    int menuchoice;
    double Bank;
    double betAmount;


    cout << "Главное меню:" << endl;
    cout << "1. Начать игру" << endl;
    cout << "2. Выход из программы" << endl;
    cout << "Выберите действие: ";
    cin >> menuchoice;
    if (menuchoice == 1)
    {
        cout << "Поставьте банк на кон: ";
        cin >> Bank;
    }
    else
    {
        return 0;
    }


    do
    {
        while (Bank > 0)
        {
            Player* player = new Player();
            Player* CPU = new Player();

            cout << "Доступный банк: " << Bank << endl;

            // Запрос игроку сделать ставку
            do {
                cout << "Делайте вашу ставку: ";
                cin >> betAmount;
                if (betAmount > Bank) {
                    cout << "У вас недостаточно средств. Попробуйте снова." << endl;
                }
                cout << endl;
            } while (betAmount > Bank);
            switch (menuchoice)
            {
            case 1:
            {
                for (int i = 0; i < 2; i++) {
                    player->getCardInhand(new Card(getRandomUniqueCard(deck)));
                    CPU->getCardInhand(new Card(getRandomUniqueCard(deck)));
                }

                cout << "Карты дилера:" << endl;
                drawPlayerHand(CPU, false);
                cout << endl << "Сумма карт: " << CPU->getSummCardInHand(false) << endl;
                cout << endl;


                cout << "Карты игрока:" << endl;
                drawPlayerHand(player, true);
                cout << endl << "Сумма карт: " << player->getSummCardInHand(true) << endl;
                cout << endl;

                if (CPU->hasBlackjack())
                {
                   
                    Bank -= betAmount;

                    cout << "Карты дилера:" << endl;
                    drawPlayerHand(CPU, false);
                    cout << endl << "Сумма карт: " << CPU->getSummCardInHand(true) << endl;
                    cout << endl;


                    cout << "Карты игрока:" << endl;
                    drawPlayerHand(player, true);
                    cout << endl << "Сумма карт: " << player->getSummCardInHand(true) << endl;
                    cout << endl;
                    cout << "BLACKJACK! ПОБЕДИЛ ДИЛЕР" << endl;
                    break;
                }
                if (player->hasBlackjack())
                {
                    
                    Bank = betAmount * 2;


                    cout << "Карты дилера:" << endl;
                    drawPlayerHand(CPU, false);
                    cout << endl << "Сумма карт: " << CPU->getSummCardInHand(true) << endl;
                    cout << endl;


                    cout << "Карты игрока:" << endl;
                    drawPlayerHand(player, true);
                    cout << endl << "Сумма карт: " << player->getSummCardInHand(true) << endl;
                    cout << endl;
                    cout << "BLACKJACK! ПОБЕДИЛ ИГРОК" << endl;
                    break;
                }
                cout << "Выберите действие: " << endl;
                cout << "1. Взять еще одну карту\n2. Пас" << endl;

                cin >> choice;

                switch (choice)
                {
                case 1:
                {
                    char nextMove;
                    do {
                        player->getCardInhand(new Card(getRandomUniqueCard(deck)));


                        cout << "Карты дилера:" << endl;
                        drawPlayerHand(CPU, false);
                        cout << endl << "Сумма карт: " << CPU->getSummCardInHand(false) << endl << endl;


                        cout << "Карты игрока:" << endl;
                        drawPlayerHand(player, true);
                        cout << endl << "Сумма карт: " << player->getSummCardInHand(true) << endl;


                        if (player->getSummCardInHand(true) > 21) {
                            cout << endl << "У вас перебор! Игра окончена.\n";
                            break;
                        }

                        cout << "Вы хотите взять ещё карту?\n1. Взять еще одну карту\n2. Пас" << endl;
                        cin >> nextMove;
                    } while (nextMove == '1');
                    cout << endl;
                }
                break;
                case 2:
                    break;
                default:
                    break;
                }

                // Логика для компьютера
                while (CPU->getSummCardInHand(true) < 17)
                {
                    for (int i = 0; i < 1; i++)
                    {
                        CPU->getCardInhand(new Card(getRandomUniqueCard(deck)));
                    }

                }

                cout << "Карты дилера:" << endl;
                drawPlayerHand(CPU, true);
                cout << endl << "Сумма карт: " << CPU->getSummCardInHand(true) << endl;
                cout << endl;


                cout << "Карты игрока:" << endl;
                drawPlayerHand(player, true);
                cout << endl << "Сумма карт: " << player->getSummCardInHand(true) << endl;


                cout << "\nРЕЗУЛЬТАТ";
                // Определение победителя
                if (player->getSummCardInHand(true) > 21)
                {
                    cout << endl << "Победил дилер!" << endl;
                    Bank -= betAmount;
                    break;
                }
                else if (CPU->getSummCardInHand(true) > 21)
                {
                    cout << endl << "Победил игрок!" << endl;
                    double winnings = betAmount / 2;
                    Bank += betAmount + winnings;
                    break;
                }
                else if (player->getSummCardInHand(true) > CPU->getSummCardInHand(true))
                {
                    cout << endl << "Победил игрок!" << endl;
                    double winnings = betAmount / 2;
                    Bank += betAmount + winnings;
                    break;
                }
                else if (player->getSummCardInHand(true) < CPU->getSummCardInHand(true))
                {
                    cout << endl << "Победил дилер!" << endl;
                    Bank -= betAmount;
                    break;
                }
                else
                {
                    cout << endl << "Ничья!" << endl;
                    break;
                }

            }
            }
            deck.clear();
            deck = initialDeck;
            delete player;
            delete CPU;
        }
        cout << "Ваш баланс: " << Bank << " \nИгра окончена.\n";
        break;
    } while (menuchoice != 2);


    return 0;
}
