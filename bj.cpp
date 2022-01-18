#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
using namespace std;

class card{
    public:
    string name;
    char symbol;
    string suit;
    int value;

    string cardString(){
        return suit + string(1, this->symbol);
    }

};

class deck{
    public:
    vector<card> cards;
    vector<card> used;
    int cardCount;
    int initialCards;

    deck(){
        makeSuit("♠");
        makeSuit("♣");
        makeSuit("♥");
        makeSuit("♦");
        cardCount = 13 * 4;
        initialCards = cardCount;
    }

    deck(int deckCount){
        cardCount = 0;
        for(int i = 0; i < deckCount; i++){
            makeSuit("♠");
            makeSuit("♣");
            makeSuit("♥");
            makeSuit("♦");
            cardCount += 13 * 4;
        }
        initialCards = cardCount;
    }

    void resize(int deckCount){
        cards.clear();
        used.clear();
        cardCount = 0;
        for(int i = 0; i < deckCount; i++){
            makeSuit("♠");
            makeSuit("♣");
            makeSuit("♥");
            makeSuit("♦");
            cardCount += 13 * 4;
        }
        initialCards = cardCount;
    }

    void shuffleDeck(){
        for(int i = 0; i < used.size(); i++){
            card tmp = used.back();
            used.pop_back();
            cards.push_back(tmp);
        }
        random_shuffle(cards.begin(), cards.end());
    }

    void printDeck(){
        for(int i = 0; i < cards.size(); i++){
            if (i % 13 == 0)
                cout << endl;
            cout << cards[i].suit << cards[i].symbol << " ";
            
        }
    }

    card drawCard(int &count){
        card tmp = cards.back();
        cards.pop_back();
        used.push_back(tmp);

        if(tmp.value <= 6)
            count += 1;
        else if (tmp.value >= 10)
            count -= 1;
        return tmp;
    }

    void sameDeck(){
        cards.clear();
        used.clear();
        cardCount = 52;
        initialCards = cardCount;
        for(int i = 0; i < 52; i++){
            card tmp;
            tmp.suit = "♠";
            tmp.value = 10;
            tmp.name = "Ten";
            tmp.symbol = 'T';
            cards.push_back(tmp);
        }
    }

    private:
    void makeSuit(string suit){
        for(int i = 2; i <= 9; i++){
            card tmp;
            tmp.name = to_string(i);
            tmp.symbol = i + '0';
            tmp.suit = suit;
            tmp.value = i;
            cards.push_back(tmp);
        }
        card tmp;
        tmp.suit = suit;
        tmp.value = 10;
        tmp.name = "Ten";
        tmp.symbol = 'T';
        cards.push_back(tmp);
        tmp.name = "Queen";
        tmp.symbol = 'Q';
        cards.push_back(tmp);
        tmp.name = "King";
        tmp.symbol = 'K';
        cards.push_back(tmp);
        tmp.name = "Jack";
        tmp.symbol = 'J';
        cards.push_back(tmp);
        tmp.name = "Ace";
        tmp.symbol = 'A';
        tmp.value = 11;
        cards.push_back(tmp);
    }

    
};

class ent_t{

    friend class blackjack;
    
    string name;
    bool human;
    int money;

    public:
    ent_t(){
        name = "";
        human = false;
        money = 0;
    }

    ent_t(string name, bool human, int money){
        this->human = human;
        this->name = name;
        this->money = money;
    }

    int getMoney(){
        return money;
    }

};

class hand{
    public:
    vector<card> handCards;
    int handCount;
    int handValue;
    int lowAceCount;
    int highAceCount;
    int lastAceValue;
    hand(){
        lowAceCount = 0;
        handValue = 0;
        highAceCount = 0;
        handCount = 0;
        lastAceValue = 0;
    }

    void addCard(card cardToAdd){
        handCards.push_back(cardToAdd);
        handCount++;
        if(cardToAdd.symbol == 'A'){
            highAceCount ++;
            lastAceValue = 11;
        }
        handValue += (cardToAdd.value);
        if(handValue > 21){
            for(int i = 0; i < highAceCount and handValue > 21; i++){
                handValue -= 10;
                lastAceValue = 1;
                highAceCount--;
                lowAceCount++;
            }
        }
    }

    void removeLastCard(){
        card tmp = handCards.back();
        if(tmp.name == "Ace"){
            handValue -= lastAceValue;
        }
        else
            handValue -= tmp.value;
        handCount--;

        handCards.pop_back();
    }

    void displayHand(){
        for(int i = 0; i < handCards.size(); i++){
            cout << handCards[i].cardString() << " ";
        }
        cout << endl;
        
        cout << "Value: " << handValue << endl;
    }
};

class blackjack {
    deck playingDeck;
    //vector<ent_t*> players;
    ent_t* player;
    int cardsUsed;
    int count;

    
    public:
        void sameRound()
        {
            playingDeck.sameDeck();
        }

    blackjack(){
        count = 0;
        cardsUsed = 0;
        playingDeck.resize(8);
        string name;
        int money = 1000;
        cout << "Enter Name: ";
        cin >> name;
        while(cin.fail()){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid name. Enter Name: ";
            cin >> name;
        }
        
        ent_t* playerTmp = new ent_t(name, true, money);
        player = playerTmp;
        /*
        int aiCount;
        cout << "Enter AI Count: ";
        cin >> aiCount;
        */
    }

    ~blackjack(){
        delete player;
    }

    void playRound(){
        if(cardsUsed % (playingDeck.initialCards/2) == 0)
            playingDeck.shuffleDeck();
        cout << "Your Money: " << player->getMoney() << endl;
        int bet;
        int stake;
        cout << "Enter Bet Amount: ";
        cin >> bet;
        while(cin.fail() or bet > player->getMoney() or bet <= 0){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid Bet. Enter Bet Amount: ";
            cin >> bet;
        }

        player->money -= bet;
        stake = bet;

        vector<hand*> playerHands;
        playerHands.push_back(new hand);
        hand playerHand;
        cardsUsed += 2;
        playerHands[0]->addCard(playingDeck.drawCard(count));
        playerHands[0]->addCard(playingDeck.drawCard(count));

        cardsUsed++;
        hand dealerHand;
        dealerHand.addCard(playingDeck.drawCard(count));

        
        
        bool done = false;
        bool canSplit = false;
        for(int i = 0; i < playerHands.size();i++){
            done = false;
            while(!done){
                cout << "Your cards: ";
                playerHands[i]->displayHand();
                cout << endl;
                cout << "Dealer hand: ";
                dealerHand.displayHand();
                cout << endl;

                cout << "Count: " << count << endl;
                if (playerHands[i]->handCount == 2 and playerHands[i]->handValue == 21){
                    cout << "Blackjack. 3 to 2 payout." << endl;
                    player->money += 3 * stake / 2;
                    break;
                }
                canSplit = false;
                done = true;
                char move;
                cout << "Hit (H), stand (S), double down (D)";
                if(playerHands[i]->handCount == 2 and playerHands[i]->handCards[0].name == playerHands[i]->handCards[1].name){
                    cout << ",Split (P)";
                    canSplit = true;
                }
                cout << "? ";
                cin >> move;
                while(cin.fail() or (move != 'H' and move != 'S' and move != 'D' and move != 'P') or (move == 'D' and bet > player->money) or (move == 'P' and canSplit == false) or (move == 'P' and player->getMoney() < bet)){
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid move. Hit (H), stand (S), double down (D)";
                    if(playerHands[i]->handCount == 2 and playerHands[i]->handCards[0].name == playerHands[i]->handCards[1].name){
                        cout << ",Split (P)";
                    }
                    cout << "? ";
                    cin >> move;
                }

                switch(move){
                    case 'H':
                        cardsUsed++;
                        playerHands[i]->addCard(playingDeck.drawCard(count));
                        if(playerHands[i]->handValue <= 21){
                            done = false;
                        }else{
                            playerHands[i]->displayHand();
                        }
                    break;
                    case 'S':

                    break;
                    case 'D':
                        player->money -= bet;
                        stake += bet;   
                    break;
                    case 'P':
                        player->money -= bet;
                        playerHands.push_back(new hand);
                        cardsUsed += 2;

                        playerHands[i+1]->addCard(playerHands[i]->handCards[1]);
                        playerHands[i+1]->addCard(playingDeck.drawCard(count));
                        playerHands[i]->removeLastCard();
                        playerHands[i]->addCard(playingDeck.drawCard(count));
                        done = false;
                        break;
                }
            }
        }
        cout << endl;

        cout << "Dealer Draw: " << endl;
        while(dealerHand.handValue < 17){
            cardsUsed++;
            dealerHand.addCard(playingDeck.drawCard(count));
            dealerHand.displayHand();
        }
        
        // check each hand here
        for(int i = 0; i < playerHands.size(); i++){
            if(playerHands[i]->handValue > 21){
                cout << "Bust" << endl;
            }
            else if(dealerHand.handValue > 21 or dealerHand.handValue < playerHands[i]->handValue){
                cout << "You win" << endl;
                player->money += (2*stake);
            }
            else if(dealerHand.handValue == playerHands[i]->handValue){
                cout << "Pushback." << endl;
                player->money += stake;
            }
            else
                cout << "You lost." << endl;
            delete playerHands[i];
        }
        return;
    }
    void displayMoney(){
        cout << player->getMoney() << endl;
    }

    bool playerMoney(){
        return player->getMoney() > 0;
    }

};


int main(){
    srand(time(NULL));
    blackjack bj;
    bj.playRound();
    cout << endl;
    bj.displayMoney();
    bool playAgain = false;
    cout << "Play Again? ";
    cin >> playAgain;
    while(playAgain and bj.playerMoney()){
        bj.playRound();
        cout << endl;
        bj.displayMoney();
        cout << "Play Again? ";
        cin >> playAgain;
    }

    return 0;
}