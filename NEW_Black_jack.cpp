#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

class Card //класс который отвечает за наличие карт
{
public:
	enum rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUINN, KING }; //тут значения карт,(enum это что бы сохранялись конст для каждой величины) 
	enum suit {CLUBS, DIAMONDS, HEARTS, SPADES}; //масти
	friend ostream& operator<<(ostream& os, const Card& a_card); //дружественный оператор перегрузки <<,который как 2 аргумент берет конст ссылку на класс -Кард-
	Card(rank r = ACE, suit s = SPADES, bool ifu = true); //конструктор,который придает начальное значение 1,пики,карта лицом вверх- правда
	int Get_value() const; //функция отображения значения карты
	void Flip(); //перевернуть карту
private:
	rank m_rank; //я так понимаю,что rank это как int/string ну в общем переменная которая будет брать в себя значения типа 1 и т.д
	suit m_suit; //ну она масти берет на себя
	bool m_is_face_up; //показывает правда ли наша карта перевернута лицом вверх
};

Card::Card(rank r, suit s, bool ifu) : m_rank(r), m_suit(s), m_is_face_up(ifu) //передаем значения
{}

int Card::Get_value() const 
{
	int value = 0; //нач значение 0
	if (m_is_face_up) //пока правда
	{
		value = m_rank; //присваиваем значение переменной m_rank
		if (value > 10)// и вот если больше 10,а у карты лежащей вверху не может быть такого,присваиваем 10
		{
			value = 10;
		}
	}
	return value; //и собственно выводим наше значение,если не подходит под наше условие
}

void Card::Flip()
{
	m_is_face_up = !(m_is_face_up); //переворачиваем нашу карту
}


class Hand //класс который представляет коллекцию карт 
{
public:
	Hand(); //конструктор который выделяет место для карт
	virtual ~Hand(); //удаляет эти карты
	int Get_total() const; //возвращает сумму карт 
	void Add(Card* p_card); //добавляет в свободный массив(m_cards) карты
	void Clear(); //очищает массив
protected:
	vector<Card*> m_cards; //содержит в себе значения карт
};

Hand::Hand()
{
	m_cards.reserve(7); //выделяем место для 7 карт
}

Hand::~Hand()
{
	Clear(); //обращаемся к функции удаления
}

void Hand::Add(Card* p_card)
{
	m_cards.push_back(p_card); //добавляем в конце вектора значение m_cards
}

void Hand::Clear()
{
	vector<Card*>::iterator iter = m_cards.begin(); //присваиваем итератору значение начала вектора
	for (iter = m_cards.begin(); iter != m_cards.end(); ++iter) //пробегаемся по всему вектору
	{
		delete* iter; //удаляем разыменнованый итератор
		*iter = 0; //присваиваем 0,чтобы случайно не разыменовать
	}

	m_cards.clear(); //очищаем все,елки иголки
}

int Hand::Get_total() const
{
	if (m_cards.empty()) //во первых не равен ли наша колода карт 0
	{
		return 0; //если да. то 0
	}

	if (m_cards[0]->Get_value() == 0) //а наша первая карта равна 0
	{ 
		return 0; //если да,то 0
	}

	int total = 0; //сумма
	vector<Card*>::const_iterator iter; //конст итератор,который позволит копаться в этом массиве
	for (iter = m_cards.begin(); iter != m_cards.end(); ++iter) //пробегаемся по вектору
	{
		total += (*iter)->Get_value(); //прибавляем разыменнованный итератор от значения функции
	}

	bool constain_ace = false; //проверяем не держит ли наша рука  туз
	for (iter = m_cards.begin(); iter != m_cards.end(); ++iter) //пробегаемся по вектору
	{
		if ((*iter)->Get_value() == Card::ACE) //если у нас значение равно 1
		{
			constain_ace = true; //то держит туз
		}
	}

	if (constain_ace && total <= 11) //если у нас маленькая сумма
	{
		total += 10; //то туз будет стоить 10
	}

	return total; //возвращаем значение,если  не под одно условие не подошло
}


class Generic_player : public Hand //класс который создает подобие игрока ,наследует все от колдоды карт
{
friend ostream& operator<<(ostream& os, Generic_player& a_generic_player);  //перегружаем оператор вывода,чтобы мы могли отобразить хоть,что нибудь
public:
	Generic_player(const string& name = ""); //конструктор,который будет передовать значение из ссылки на строку в приват
	virtual ~Generic_player(); //диконструктор
	virtual bool Is_hitting() const = 0; //чистая функция,виртуальная,чтобы можно было ее использовать дальше и по разному
	bool is_busted() const; //выводит сумму карт ,большую 21
	void bust() const; //объявляет,что у игрока перебор
protected:
	string m_name; //хранит в себе ссылку на символ
}; 

Generic_player::Generic_player(const string& name) : m_name(name) //осуществление конструктора
{}

Generic_player::~Generic_player()
{}

bool Generic_player::is_busted() const //выводит сумму,большую 21
{
	return (Get_total() > 21); 
}

void Generic_player::bust() const //объявляет что у игрока перебор
{
	cout << m_name << " busts\n";
}

class Player : public Generic_player // класс игрока-человека,наследующий все от generic_player
{
public:
	Player(const string& name = ""); //констурктор который будет обновлять переменную m_name из класса generic_player (берет ссылку на строку)
	virtual ~Player(); //просто деструктор
	virtual bool Is_hitting() const; //объявление нашей виртуальной функции,которая спрашивает у игрока хочет ли он взять карту
	void Win() const; //объявляет победителя
	void Lose() const; //объявляет проигравшего
	void Push() const; //объявляет ничью
};

Player::Player(const string& name) : Generic_player(name) //обновляем значение переменной m_name
{}

Player::~Player()
{}

bool Player::Is_hitting() const
{
	cout << m_name << " do u want a hit? (y/n) :"; //спрагиваем хочет ли он взять карту(игрок)
	char response;
	cin >> response; 
	return (response == 'y' || response == 'Y'); //возвращаем true если да 
}

void Player::Win() const //победитель
{
	cout << m_name << " wins!\n";
}

void Player::Lose() const //проигравший
{
	cout << m_name << " loses.\n";
}

void Player::Push() const //ничья
{
	cout << m_name << " pushes.\n";
}

class House : public Generic_player //подразумевает собой диллера наследует все от класса generic_player
{
public:
	House(const string& name = "House"); //конструктор,нач значение которого это имя house
	virtual ~House(); //деструктор
	virtual bool Is_hitting() const; //проверяет может ли диллер брать карту
	void Flip_first_card() const; //переворачивает первую карту
};

House::House(const string& name) : Generic_player(name) //обновляем значение переменной m_name из класса generic_player
{}

House::~House()
{}

bool House::Is_hitting() const //как раз проверяем может ли диллер брать карту
{
	return (Get_total() <= 16);
}

void House::Flip_first_card() const 
{
	if (!(m_cards.empty())) //если наша колода не пустая,то мы можем брать карту
	{
		m_cards[0]->Flip(); //первую карту переворачиваем с помощью функции flip()
	}
	else
	{
		cout << "No card to flip!\n"; //если что пишем что карт нет
	}

}

class Deck : public Hand //класс который предстовляет колоду карт и наследует от класса hand
{
public:
	Deck(); //конструктор,который будет выделять место для 52 карт
	~Deck(); //деструктор
	void Shuffle(); //будет перемешивать колоду
	void Populate(); //будет создавать колоду,пробегаясь по всему словарю значений и масетй
	void Deal(Hand& a_hand); //будет передовать карту в руку
	void Additional_cards(Generic_player& a_generic_player); //доп карта
};

Deck::Deck()
{
	m_cards.reserve(52); //выделяем место
	Populate(); //обращаемся к функции колоды
}

Deck::~Deck()
{}

void Deck::Populate() //создаем колоду
{
	Clear();
	for (int s = Card::CLUBS; s <= Card::SPADES; ++s) //пробегаемся по мастям
	{
		for (int r = Card::ACE; r <= Card::KING; ++r) //по значениям 
		{
			Add(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(r))); //добавляем,выделяя место,сначала значение,а затем уже масть
		}
	}
}

void Deck::Shuffle()
{
	random_shuffle(m_cards.begin(), m_cards.end()); //перемешиваем всю колоду
}

void Deck::Deal(Hand& a_hand) //передаем карту в руку
{
	if (!m_cards.empty()) //не пустая ли наша колода
	{
		a_hand.Add(m_cards.back()); //добавляем в нашу переменную последнее значение из коллекции наших карт
		m_cards.pop_back(); //и сразу удаляем это значение
	}

	else
	{
		cout << "Out the cards.\n"; //иначе не может дать карты
	}
}

void Deck::Additional_cards(Generic_player& a_generic_player) //хочет ли игрок брать доп карту
{
	cout << endl;
	while (!(a_generic_player.is_busted() && a_generic_player.Is_hitting())) //пока у игрока сумма карт меньше 21 и он хочет брать
	{
		Deal(a_generic_player); //даем ему
		cout << a_generic_player << endl; //выводим значение полученной карты
		if (a_generic_player.is_busted()) //если у нашего игрока стал перебор
		{
			a_generic_player.bust(); //объявляем ему об этом
		}
	}
}

class Game 
{
public:
	Game(const vector<string>& names); //конструктор,который будет отвечать за то,что добавляет именя в список игроков
	~Game(); //ерунда какая-то деструктор
	void Play() ; //осуществляет игру в blackjack
private:
	Deck m_deck; //переменная хранящая в себе значение колоды
	House m_house; //карты у диллера
	vector<Player> m_players; //имена игроков
};

Game::Game(const vector<string>& names)
{
	vector<string>::const_iterator p_name; //итератор,для того,что мы бы могли копаться в пространстве имен
	for (p_name = names.begin(); p_name != names.end(); ++p_name)
	{
		m_players.push_back(Player(*p_name)); //мы добавляем наше имя игрока в конец вектор
	}

	srand(static_cast<unsigned int>(time(0))); //функция рандома для последующих действиях с картами-
	m_deck.Populate(); //1 создать колоду
	m_deck.Shuffle(); //перемешать данную колоду

}

Game::~Game()
{}

void Game::Play()
{
	vector<Player>::iterator p_player; //итератор,для --копания-- в именах игроков
	for (int i = 0; i < 2; ++i) //раздаем игроку 2 карты
	{
		for (p_player = m_players.begin(); p_player != m_players.end(); ++p_player) //проходим по всем игрокам
		{
			m_deck.Deal(*p_player); //если дословно,то из колоды передаем карту в руку игроку(разыменнованный - значит  с именем)
		}
		m_deck.Deal(m_house); //передаем карту дилеру
	}

	m_house.Flip_first_card(); //переворачиваем первую карту дилера
	for (p_player = m_players.begin(); p_player != m_players.end(); ++p_player) //выводим значение всех карт игроков
	{
		cout << *p_player << endl; 
	}
	cout << m_house << endl; //выводим значение карты диллера
	
	for (p_player = m_players.begin(); p_player != m_players.end(); ++p_player) //нужна ли игроку доп карта на руки
	{
		m_deck.Additional_cards(*p_player); //добавляем в коллекцию карт выбор от игрока,берет или нет
	}
	m_house.Flip_first_card(); //переворачиваем карту диллера,чтобы отобразить значение
	cout << endl << m_house; 
	m_deck.Additional_cards(m_house); //нужна ли доп карта диллеру

	if (m_house.is_busted()) //проверяем а не проиграл ли диллер
	{
		for (p_player = m_players.begin(); p_player != m_players.end(); ++p_player) //проходим по всему списку
		{
			if (!(p_player->is_busted())) //если никто из игроков не проиграл
			{
				 p_player->Win(); //выводим игроков победителями
			}
		}
	}
	else
	{
		for (p_player = m_players.begin(); p_player != m_players.end(); ++p_player) //снова по списку
		{
			if (!(p_player->is_busted())) //пока из игроков никто не проиграл
			{
				if (p_player->Get_total() > m_house.Get_total()) //и сумма очков больше суммы очков диллера
				{
					p_player->Win(); //выводим победителей
				}
				else if (p_player->Get_total() < m_house.Get_total()) //если меньше,то проигрыш
				{
					p_player->Lose();
				}
				else //иначе ничья
				{
					p_player->Push();
				}
			}
		}
	}
	
	for (p_player = m_players.begin(); p_player != m_players.end(); ++p_player) //очищаем нашу колоду
	{
		p_player->Clear(); //у игрока очищаем
	}
	m_house.Clear(); //у дилера очищаем
}

ostream& operator<<(ostream& os, const Card& a_card);
ostream& operator<<(ostream& os,  Generic_player& a_generic_player);
int main()
{
	cout << "\t\tWelcome tooo ..... BLACKJACK!\n\n";
	int num_players = 0; //кол-во игроков
	while (num_players < 1 || num_players > 7) //пока пользователь не введет какое то значение,цикл будет работать!!
	{
		cout << "How many players? (1 - 7) : "; //просим ввести кол-во игроков
		cin >> num_players; //считываем
	}

	vector<string> names; //переменная хрянящая имена игроков
	string name; //имя игрока
	for (int i = 0; i < num_players; ++i) //пока меньше кол-ва игроков
	{
		cout << "Enter a name: "; //просим ввести
		cin >> name;
		names.push_back(name); //добавляем в наш вектор
	}
	cout << endl;

	Game a_game(names); //цикл игровой
	char again = 'y'; //правильный символ,который должен ввести пользователь//
	while (again != 'n' && again != 'N') //цикл пока пользователь не нажмет нет
	{
		a_game.Play();
		cout << "Wanna play again? (y/n) ";
		cin >> again;
	}

	return 0;
}

ostream& operator<<(ostream& os,const Card& a_card)
{
	const string RANKS[] = { "0" , "A" , "1" , "2" ,"3" , "4" , "5" , "6" , "7" , "8" , "9" , "10" , "J" , "Q" , "K" }; //значение карт
	const string SUITS[] = { "c" , "d", "h" , "s" }; //масти карт
	if (a_card.m_is_face_up) //если карта лицом вверх
	{
		os << RANKS[a_card.m_rank] << SUITS[a_card.m_suit]; //то выводим ее масть и значение,точнее не всю масть а только букву
	}

	else
	{
		os << "XX"; //иначе выводим ХХ
	}
	return os;
}

ostream& operator<<(ostream& os,  Generic_player& a_generic_player)
{
	os << a_generic_player.m_name << "\t:"; //выводим имя игрока
	vector<Card*>::const_iterator p_card; //создаем указатель 

	if (!a_generic_player.m_cards.empty())
	{
		for (p_card = a_generic_player.m_cards.begin();p_card != a_generic_player.m_cards.end(); ++p_card)
		{
			os << (*(*p_card)) << "\t";
		}
		if (a_generic_player.Get_total() != 0)
		{
			os << "(" << a_generic_player.Get_total() << ")";
		}
	}
	else
	{
		os << "<empty>";
	}
	return os;
}









