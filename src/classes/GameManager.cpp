#include "GameManager.h"
#include "MonsterManager.h"

Dungeon* GameManager::activeDungeon;
Player GameManager::player;
LinkedList<Dungeon> GameManager::dungeonList;
GameState GameManager::gameState;

void GameManager::startNewGame() {

   // create new player
   player = Player("test");

   // generate map
   LinkedList<Dungeon> newList;
   dungeonList = newList;

   if (MonsterManager::getMonsterCount() > 0) {
      for (int i = 0; i < 20; i++) {
         int randomIndex = rand() % MonsterManager::getMonsterCount();

         Monster* newMonster =
             new Monster(MonsterManager::getMonsterAtIndex(randomIndex));

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
   int randomNum = rand() % 20;

   // check  if its valid index
   if (randomNum < dungeonList.getLength()) {
      activeDungeon = &dungeonList[randomNum];
   }

   if (activeDungeon->getMonster()->getHp() < 0) {
      gameState = GameState::D20;
   } else {
      gameState = GameState::D10_ENEMY;
   }

   return randomNum;
}

int GameManager::playerAttack() {
   int randomNum = (rand() % 10) + 1;

   activeDungeon->getMonster()->takeDamage(randomNum);

   // if killed monster add it to list
   if (activeDungeon->getMonster()->getHp() <= 0) {
      gameState = GameState::D8;
      player.addDefeatedMonster(*(activeDungeon->getMonster()));
      // player.addDefeatedMonster(Monster(*(activeDungeon->getMonster())));

   } else {
      gameState = GameState::D10_ENEMY;
   }

   return randomNum;
}

int GameManager::enemyAttack() {
   int randomNum = (rand() % 10) + 1;

   player.setHp(player.getHp() - randomNum);

   if (player.getHp() <= 0)
      gameState = GameState::DEAD;
   else
      gameState = GameState::D10_PLAYER;

   return randomNum;
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