#include<iostream>
#include<string>
#include<vector>
#define AP_dmg 1
#define AD_dmg 0
using namespace std;

class Game_Character{
public:
    Game_Character(int _health, int _mana, int _armor, int _magicRst, int _AD, int _AP, string _name) 
    : max_health(_health), mana(_mana), armor(_armor), magicRst(_magicRst), AD(_AD), AP(_AP), name(_name)
    {
        health = max_health;
    }
    virtual void printStatus() = 0;                                         //ok
    void damage(int enemy_attack, int enemy_lastwhisper, int type);   //ok
    int basic_attack();                       //ok
    int get_lastwhisper();                    //ok
    int get_force_damage();                 //ok
    int get_force_damage_type();            //ok
    bool isAlive();                         //ok

protected:
    int max_health;
    int health;
    int mana;
    int armor;
    int buffed_armor = 0;              //버프먹은 방어력
    int magicRst;
    int buffed_magicRst = 0;           //버프먹은 마저
    int AD;                            //기본 AD
    int buffed_AD = 0;                 //버프나 CC먹은 AD
    int AP;
    int lastwhisper = 0;               //방무 0이면 방무가 없는거고 1이면 완전 트루뎀
    int game_turn = 0;                 //스킬이나 패시브 발동하는데 필요한 턴 제공
    int force_damage = 0;              //상대방에게 가하는 데미지
    int force_damage_type;             //상대방에게 가하는 데미지 타입
    bool loyalgaurd = false;           //무적기 사용 여부
    string name;                       //객체 name
};

void Game_Character::damage(int enemy_attack, int enemy_lastwhisper, int type){
    if(loyalgaurd){
        //no damage
    }
    
    else if(type == AD_dmg){
        if(enemy_lastwhisper == 1){
            health -= enemy_attack;   //방깎 있으면 트루뎀, 플젝에선 방관 가진게 아처밖에 없으니 이렇게 해도 됨
        }
        else{
            int temp = (double)enemy_attack*(100-100*(((double)(armor+buffed_armor)/(100+(double)(armor+buffed_armor)))))/100;
            health -= temp;
        }
    }

    else if(type == AP_dmg){
       int temp = (double)enemy_attack*(100-100*(((double)(magicRst+buffed_magicRst)/(100+(double)(magicRst + buffed_magicRst)))))/100;
       health -= temp;
    }

    if(health < 0){
        health = 0;
    }
}

int Game_Character::basic_attack(){
    return AD + buffed_AD;
}

int Game_Character::get_lastwhisper(){
    return lastwhisper;
}

int Game_Character::get_force_damage(){
    return force_damage;
}

int Game_Character::get_force_damage_type(){
    return force_damage_type;
}

bool Game_Character::isAlive(){
    if(health > 0)
        return true;
    else
        return false;
}


class Hero : public Game_Character{
public:
    Hero(int _health, int _mana, int _armor, int _magicRst, int _AD, int _AP, string _name) 
    : Game_Character(_health, _mana, _armor, _magicRst, _AD, _AP, _name){};
    virtual void passive() = 0;         //ok
    virtual void attack() = 0;               // ok
    void printStatus();                 // ok
    static int get_numHeros();              // ok          
    static void set_numHeros(int a);        //ok

protected:
    static int numHeros;
};
int Hero::numHeros = 0;

int Hero::get_numHeros(){
    return numHeros;
}

void Hero::printStatus(){
    cout << name << ", health: " << health << ", mana: " << mana << endl;

}

void Hero::set_numHeros(int a){
    numHeros = a;
}



class Warrior : public Hero{
public:
  Warrior(int _health, int _mana, int _armor, int _magicRst, int _AD, int _AP)
    : Hero(_health, _mana, _armor, _magicRst, _AD, _AP, "Warrior"){};

    void passive(){ 
        game_turn ++;
        if(get_numHeros() >= 2){
            buffed_AD = AD*0.15;
        }
        else{
            buffed_AD = 0;
        }
       
    }

    void attack(){
        if(mana >= 30 && game_turn % 4 == 0){
            mana -= 30;
            loyalgaurd = true;
        }
        else{
            loyalgaurd = false;
        }
        force_damage = basic_attack();  // warrior는 공격스킬이 없음
        force_damage_type = AD_dmg;     // 공격 타입은 AD
    }

};

class Archer : public Hero{
public:
  Archer(int _health, int _mana, int _armor, int _magicRst, int _AD, int _AP)
    : Hero(_health, _mana, _armor, _magicRst, _AD, _AP, "Archer"){};

    void passive(){
        game_turn ++;
        lastwhisper = 1;   //방관 활성화
    }

    void attack(){
        if(mana>=15 && game_turn % 3 == 0){
            lastwhisper = 0;  //평타 방관 비활성
            mana -= 15;
            force_damage = 20+AD*0.2;
            force_damage_type = AD_dmg;
        }

        else{
            force_damage = basic_attack();
            force_damage_type = AD_dmg;
        }
    }
};

class Wizard : public Hero{
public:
  Wizard(int _health, int _mana, int _armor, int _magicRst, int _AD, int _AP)
    : Hero(_health, _mana, _armor, _magicRst, _AD, _AP, "Wizard"){};

    void passive(){
        game_turn ++;
        if(game_turn % 2 == 0){
            mana += 5;
        }
    }

    void attack(){
        if(mana>=10){
            mana -= 10;
            force_damage = 20+AP*0.3;
            force_damage_type = AP_dmg;
        }

        else{
            force_damage = basic_attack();
            force_damage_type = AD_dmg;
        }
    }
};



class Monster : public Game_Character{
public:
    Monster(int _health, int _mana, int _armor, int _magicRst, int _AD, int _AP, string _name) 
    : Game_Character(_health, _mana, _armor, _magicRst, _AD, _AP, _name)
    {
        id = id_asignator;
        id_asignator++;
    }
    virtual void passive() = 0;         
    virtual void attack() = 0;               
    void printStatus();                        // ok
    static int get_numMonsters();                // ok  
    static void set_numMonsters(int a);        //ok

protected:
    static int id_asignator;
    static int numMonsters;
    int id;
};
int Monster::id_asignator = 100;
int Monster::numMonsters = 0;

int Monster::get_numMonsters(){
    return numMonsters;
}

void Monster::printStatus(){
    cout << name << id << ", health: " << health << ", mana: " << mana << endl;
}

void Monster::set_numMonsters(int a){
    numMonsters = a;
}

class Oak : public Monster{
public:
    Oak() : Monster(50,0,15,10,7,0,"Oak"){};

    void passive(){
        game_turn ++;
        if(health < max_health/2){
            buffed_AD = AD*0.2;
        }

        else{
            buffed_AD = 0;
        }
    }

    void attack(){
            force_damage = basic_attack(); //oak는 스킬이 없다
            force_damage_type = AD_dmg;
    }
};

class Golem : public Monster{
public:
    Golem() : Monster(80,0,30,0,10,0,"Golem"){};

    void passive(){
        game_turn ++;
        max_health = max_health + 10;
        health  = health + 10;
    }

    void attack(){
            force_damage = basic_attack(); //oak는 스킬이 없다
            force_damage_type = AD_dmg;
    }
};

class Dragon : public Monster{
public:
    Dragon() : Monster(200,100,30,30,15,15,"Dragon"){};

    void passive(){
        game_turn ++;
        if(health < max_health/2){
            buffed_armor = armor*0.3;
            buffed_magicRst = magicRst*0.3;
        }

        else{
            buffed_armor = 0;
            buffed_magicRst = 0;
        }
    }

   void attack(){
        if(game_turn %5 == 0 && mana >= 30){
            mana -= 30;
            force_damage = 30+AP*0.2;
            force_damage_type = AP_dmg;
        }

        else{
            force_damage = basic_attack();
            force_damage_type = AD_dmg;
        }
    }
};

class Battle{
public:
    void start(){
        print_all_character();
        vector<Hero*>::iterator hero_iter;
        vector<Monster*>::iterator monster_iter;

        while(does_game_end() != true){
            turn++;
            hero_iter = heros.begin();
            monster_iter = monsters.begin();
            for(hero_iter; hero_iter != heros.end(); hero_iter++){
                (*hero_iter) -> passive();
            }
            for(monster_iter; monster_iter != monsters.end(); monster_iter++){
                (*monster_iter) -> passive();
            }          

            hero_iter = heros.begin();
            monster_iter = monsters.begin();
            //battle phase___ hero turn
            for(hero_iter; hero_iter != heros.end(); hero_iter++){
                (*hero_iter) -> attack();
                (*monster_iter) -> damage((*hero_iter)->get_force_damage(), (*hero_iter)->get_lastwhisper(), (*hero_iter)->get_force_damage_type());
                if((*monster_iter) -> isAlive() != true){
                    monsters.erase(monster_iter);
                    if(does_game_end()){
                        break;
                    }
                }
            }
            if(does_game_end()){
                break;
            }

            hero_iter = heros.begin();
            monster_iter = monsters.begin();
            //battle phase___ monster turn
            for(monster_iter; monster_iter != monsters.end(); monster_iter++){
                (*monster_iter) -> attack();
                (*hero_iter) -> damage((*monster_iter)->get_force_damage(), (*monster_iter)->get_lastwhisper(), (*monster_iter)->get_force_damage_type());
                if((*hero_iter) -> isAlive() != true){
                    heros.erase(hero_iter);
                    if(does_game_end()){
                        break;
                    }
                }
            }
            if(does_game_end()){
                break;
            }
            print_all_character();
        }
        print_all_character();
        if(heros.size() != 0){
            cout << "Heros win!";
        }
        else
            cout << "Monsters win!";

    }

    bool does_game_end(){
        if(heros.size() == 0 || monsters.size() == 0){
            return true;
        }
        return false;
    }

    void set_data_hero(Hero* temp){
        heros_all.push_back(temp);
        heros.push_back(temp);
    }
    void set_data_monster(Monster* temp){
        monsters_all.push_back(temp);
        monsters.push_back(temp);
    }
    void print_all_character(){
        vector<Hero*>::iterator hero_iter;
        hero_iter = heros_all.begin();
        vector<Monster*>::iterator monster_iter;
        monster_iter = monsters_all.begin();   

        if(turn != 0){
            cout << "===== Turn " << turn << "=====" << endl;
        }
        //# Heros
        cout << "# Heros" << endl;
        for(hero_iter; hero_iter != heros_all.end(); hero_iter++){
            (*hero_iter)->printStatus();
        }   
        //# Monsters
        cout << "# Monsters" << endl;
        for(monster_iter; monster_iter != monsters_all.end(); monster_iter++){
            (*monster_iter)->printStatus();
        }     
    }

private:
vector<Hero*> heros_all;
vector<Monster*> monsters_all;
vector<Hero*> heros;
vector<Monster*> monsters;
int turn = 0;
};



int main(){
    Battle  *game_start = new Battle();

    int hero_num = -1;
    cout << "Enter number of heros you want to create: ";
    cin >> hero_num;
    Hero::set_numHeros(hero_num);
    if(hero_num<=0 || hero_num >=4){
        cout << "The number is not valid.";
        delete game_start;
        exit(0);
    }

    Hero **games_heros = new Hero*[3];
    for(int i=0; i < Hero::get_numHeros() ; i++){
        int select = -1;
        cout << "Hero Number" << endl << " 1.Warrior" << endl << " 2.Archer" << endl << " 3.Wizard" << endl;
        cout << "Which hero would you choose: ";
        cin >> select;
        if(select<=0 || select >=4){
            cout << "The number is not valid.";
            delete game_start;
            delete [] games_heros;
            exit(0);
        }

        string inputstat;
        string string_stat[6];
        int stat_arr[6];
        int pivot = 0;
        int input_turn = 0;  //숫자받을 차례때 0, 컴마받을 수 있을 차례때 1
        cout << "Enter hero's stat: ";
        cin >> inputstat;

        for(int i=0; i < inputstat.size(); i++){
            if(((inputstat[i] - '0') >= 0 && (inputstat[i] - '0') <=9) || inputstat[i] == ','){
               if(inputstat[i] == ','){
                   if(input_turn == 1){
                       pivot += 1;
                       input_turn = 0;
                   }
                   else{
                        cout << "The number is not valid.";
                        delete game_start;
                        delete [] games_heros;
                        exit(0);
                   }
               }

               else{
                   string_stat[pivot] += inputstat[i];
                   input_turn = 1;
               }
            }
            else{
                cout << "The number is not valid.";
                delete game_start;
                delete [] games_heros;
                exit(0);
            }
        }

        if(pivot != 5 || input_turn == 0){
            cout << "The number is not valid.";
            delete game_start;
            delete [] games_heros;
            exit(0);
        }

        for(int i=0; i<6 ; i++){
            stat_arr[i] = stoi(string_stat[i]);
        }

        switch(select){
            case 1:
                games_heros[i] = new Warrior(stat_arr[0], stat_arr[1], stat_arr[2], stat_arr[3], stat_arr[4], stat_arr[5]);
                break;
            case 2:
                games_heros[i] = new Archer(stat_arr[0], stat_arr[1], stat_arr[2], stat_arr[3], stat_arr[4], stat_arr[5]);
                break;
            case 3:
                games_heros[i] = new Wizard(stat_arr[0], stat_arr[1], stat_arr[2], stat_arr[3], stat_arr[4], stat_arr[5]);
                break;
            }
    }


    int monster_num = -1;
    cout << "Enter the number of monster you want to create: ";
    cin >> monster_num;
    if(monster_num <= 0){
        cout << "The number is not valid.";
        delete game_start;
        for(int i=0 ; i < hero_num ; i++){
            delete games_heros[i];
        }
        delete [] games_heros;
        exit(0);
    }
    Monster **games_monsters = new Monster*[monster_num];

    int monster_i=0;
    int a;
    cout << "How many Oak do you want to create: ";
    cin >> a;
    if(a<0 || a>monster_num){
        cout << "The number is not valid.";
        delete game_start;
        for(int i=0 ; i < hero_num ; i++){
            delete games_heros[i];
        }
        delete [] games_heros;
        delete [] games_monsters;
        exit(0);
    }
    for(monster_i; monster_i < a ; monster_i++){
        games_monsters[monster_i] = new Oak();
    }

    //오크로 다 채움
    if(monster_num == monster_i){
        cout << "No more Golem can be created." << endl;
        cout << "No more Dragon can be created." << endl;
    }
    //오크로 다 안채워지면
    else{
        cout << "How many Golem do you want to create: ";
        cin >> a;
        if(a<0 || a>monster_num-monster_i){
            cout << "The number is not valid.";
            delete game_start;
            for(int i=0 ; i < hero_num ; i++){
                delete games_heros[i];
            }
            delete [] games_heros;
            delete [] games_monsters;
            exit(0);
        }
        for(int i=0; i < a ; i++){
            games_monsters[monster_i] = new Golem();
            monster_i++;
        }
        
        // 오크 골렘으로 다 채움
        if(monster_num == monster_i){
            cout << "No more Dragon can be created." << endl;
        }

        else{
            cout <<"All remaining numbers are used to create Dragon." << endl;
            for(monster_i; monster_i < monster_num ; monster_i++){
                games_monsters[monster_i] = new Dragon();
            }
        }
    }

    //BATTLE에 집어넣기
    for(int i=0 ; i < hero_num ; i++){
        game_start->set_data_hero(games_heros[i]);
    }
    for(int i=0 ; i < monster_num ; i++){
        game_start->set_data_monster(games_monsters[i]);
    }

    game_start->start();

    //delete zone
    delete game_start;
    for(int i=0 ; i < hero_num ; i++){
        delete games_heros[i];
    }
    delete [] games_heros;

    for(int i=0 ; i < monster_num ; i++){
        delete games_monsters[i];
    }
    delete [] games_monsters;


    return 0;
}