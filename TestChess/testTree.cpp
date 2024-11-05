#include <gtest/gtest.h>
#include <SFML/Graphics.hpp> // Ensure you include your necessary headers
#include <Engine.h>
#include <Game.h>

class CreateTreeTest : public ::testing::Test {
public:
	Game game;
	Engine* eng_test;
	sf::RenderWindow window;
	int counter;
	int depth;
	Move best;

	virtual void SetUp() override {
		eng_test = new Engine(true);
		depth = 4; // Set depth for testing
		auto best_val = eng_test->createTree(game.board, depth, best); // Call once
		std::cout << best_val << std::endl;
	}
	virtual void TearDown() override {
		delete eng_test; // Clean up after test
	}
};

TEST_F(CreateTreeTest, ValidatesAllCounts) {
	int expectedNumpos, expectedEnpass, expectedChecks, expectedCheckmates, expectedCaptures;

	switch (depth) {
	case 1: expectedNumpos = 20; expectedEnpass = 0; expectedChecks = 0; expectedCheckmates = 0; expectedCaptures = 0;break;
	case 2: expectedNumpos = 400; expectedEnpass = 0; expectedChecks = 0; expectedCheckmates = 0; expectedCaptures = 0;break;
	case 3: expectedNumpos = 8902; expectedEnpass = 0; expectedChecks = 12; expectedCheckmates = 0; expectedCaptures = 34;break;
	case 4: expectedNumpos = 197281; expectedEnpass = 0; expectedChecks = 469; expectedCheckmates = 8; expectedCaptures = 1576;break;
	case 5: expectedNumpos = 4865609; expectedEnpass = 258; expectedChecks = 27351; expectedCheckmates = 347; expectedCaptures = 82719;break;
	case 6: expectedNumpos = 119060324; expectedEnpass = 5248; expectedChecks = 809099; expectedCheckmates = 10828; expectedCaptures = 2812008;break;
	default: expectedNumpos = 0; expectedEnpass = 0; expectedChecks = 0; expectedCheckmates = 0; expectedCaptures = 0; break;
	}

	// Assertions for all counts in one test
	EXPECT_EQ(eng_test->positionctr, expectedNumpos) << "Incorrect position count.";
	EXPECT_EQ(game.board.enpassantctr, expectedEnpass) << "Incorrect en passant count.";
	EXPECT_EQ(eng_test->checkctr, expectedChecks) << "Incorrect check count.";
	EXPECT_EQ(eng_test->capturectr+game.board.enpassantctr, expectedCaptures) << "Incorrect capture count.";
	EXPECT_EQ(eng_test->checkmatectr,expectedCheckmates) << "Incorrect checkmate count.";
}
