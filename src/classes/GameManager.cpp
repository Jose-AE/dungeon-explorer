#include "GameManager.h"
Dungeon* GameManager::activeDungeon;
Player GameManager::player;
LinkedList<Dungeon> GameManager::dungeonList;
LinkedList<Spell> GameManager::spellList;
LinkedList<Monster> GameManager::monsterList;
GameState GameManager::gameState;

void GameManager::startNewGame(string name, string race) {

   // create new player
   player = Player(name, race);

   // generate map
   LinkedList<Dungeon> newList;
   dungeonList = newList;

   if (monsterList.getLength() > 0) {
      for (int i = 0; i < DUNGEON_COUNT; i++) {
         int randomIndex = rand() % monsterList.getLength();

         Monster* newMonster = new Monster(monsterList[randomIndex]);

         Dungeon newDungeon(newMonster);
         dungeonList.insert(newDungeon);
      }
   }

   // set game state
   gameState = GameState::D20;
}

Dungeon GameManager::getDungeonAtIndex(int index) { return dungeonList[index]; }

void GameManager::generateMap() {}

int GameManager::moveToRandomDungeon() {
   int randomNum = rand() % DUNGEON_COUNT;

   // check  if its valid index
   if (randomNum < dungeonList.getLength()) {
      activeDungeon = &dungeonList[randomNum];
   }

   if (activeDungeon->getMonster()->getHp() <= 0) {
      gameState = GameState::D20;
   } else {
      gameState = GameState::D10_ENEMY;
   }

   return randomNum;
}

int GameManager::playerAttack() {
   int randomNum = (rand() % 10) + 1;

   activeDungeon->getMonster()->takeDamage(randomNum + player.getHp());

   // if killed monster add it to list
   if (activeDungeon->getMonster()->getHp() <= 0) {

      player.addDefeatedMonster(*(activeDungeon->getMonster()));
      // player.addDefeatedMonster(Monster(*(activeDungeon->getMonster())));

      if (player.getDefeatedMonsters().getLength() == DUNGEON_COUNT) {
         gameState = GameState::WIN;
      } else {
         gameState = GameState::D8;
      }

   } else {
      gameState = GameState::D10_ENEMY;
   }

   return randomNum;
}

Player GameManager::getPlayer() { return player; }

int GameManager::enemyAttack() {
   int randomNum = (rand() % 10) + 1;

   player.setLp(player.getLp() - randomNum);

   if (player.getLp() <= 0)
      gameState = GameState::DEAD;
   else
      gameState = GameState::D10_PLAYER;

   return randomNum;
}

Spell GameManager::useItem(int itemIndex) {
   Spell spell = spellList[itemIndex];
   spellList.remove(itemIndex);

   if (spell.getType() == "Healing") {
      player.setLp(player.getLp() + spell.getValue());
   } else {
      player.setHp(player.getHp() + spell.getValue());
   }
   return spell;
}

int GameManager::recoverPlayerLP() {
   int randomNum = (rand() % 8) + 1;

   player.setLp(player.getLp() + randomNum);

   gameState = GameState::D20;

   return randomNum;
}

Dungeon* GameManager::getActiveDungeon() { return activeDungeon; }

GameState GameManager::getGameState() { return gameState; }
void GameManager::setGameState(GameState state) { gameState = state; }
int GameManager::getPlayerHp() { return player.getHp(); }

int GameManager::getPlayerLp() { return player.getLp(); }

LinkedList<Monster> GameManager::getMonsterList() { return monsterList; }

void GameManager::addMonsterToList(Monster monster) {
   monsterList.insert(monster);
}

LinkedList<Spell> GameManager::getItemList() { return spellList; }

void GameManager::addItemToList(Spell spell) { spellList.insert(spell); }
