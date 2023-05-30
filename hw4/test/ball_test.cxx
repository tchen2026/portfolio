#include "ball.hxx"
#include <catch.hxx>

Block const The_Paddle {100, 400, 100, 20};

TEST_CASE("Ball::Ball")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK(ball.center.x == 150);
    CHECK(ball.center.y == 394);
}

TEST_CASE("Ball::hits_side")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK_FALSE(ball.hits_side(config));
    ball.center.x = 1;
    CHECK(ball.hits_side(config));
    ball.center.x = config.scene_dims.width - 1;
    CHECK(ball.hits_side(config));
    ball.center.x = config.scene_dims.width / 2;
    CHECK_FALSE(ball.hits_side(config));
}

//
// "WRITE MORE TESTS!!!" is never bad advice
//

TEST_CASE("Ball::hits_top")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK_FALSE(ball.hits_top(config));
    ball.center.y = -1;
    CHECK(ball.hits_top(config));
    //ball.center.y = ball.radius;
   // CHECK(ball.hits_top(config));
    ball.center.y = 0;
    CHECK(ball.hits_top(config));
    ball.center.y = config.scene_dims.height/2;
    CHECK_FALSE(ball.hits_top(config));
}

TEST_CASE("Ball:: hits_bottom")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK_FALSE(ball.hits_bottom(config));

    ball.center.y = config.scene_dims.height + ball.radius + 1;
    CHECK(ball.hits_bottom(config));

    ball.center.y = config.scene_dims.height - ball.radius - 1;
    CHECK_FALSE(ball.hits_bottom(config));

    ball.center.y = config.scene_dims.height/2;
    CHECK_FALSE(ball.hits_bottom(config));
}

TEST_CASE("Ball::hits_block")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.center.x = 10.0f;
    ball.center.y = 10.0f;
    ball.radius = 1.0f;

    Block block;
    block.x = 8.0f;
    block.y = 8.0f;
    block.width = 4.0f;
    block.height = 4.0f;

    CHECK(ball.hits_block(block));
    ball.center.y = 5.0f;
    CHECK_FALSE(ball.hits_block(block));
    ball.center.y = 15.0f;
    CHECK_FALSE(ball.hits_block(block));
    ball.center.y = 10.0f;
    ball.center.x = 3.0f;
    CHECK_FALSE(ball.hits_block(block));
    ball.center.x = 20.0f;
    CHECK_FALSE(ball.hits_block(block));
}
TEST_CASE("ball hits corner")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.center.y = 0;
    ball.center.x = 0;
    CHECK(ball.hits_top(config));
    CHECK(ball.hits_side(config));

}
TEST_CASE("not touching the edges")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 3;
    ball.velocity = {2, -30};
    ball.center = {450, 546};
    ball.live = true;

    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_bottom(config));
    CHECK_FALSE(ball.hits_side(config));
}

TEST_CASE("detect collision of ball with sides")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {9, 300};
    ball.live = true;
    CHECK(ball.hits_side(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {10, 300};
    ball.live = true;
    CHECK_FALSE(ball.hits_side(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {1014, 300};
    ball.live = true;
    CHECK_FALSE(ball.hits_side(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {1015, 300};
    ball.live = true;
    CHECK(ball.hits_side(config));

}

TEST_CASE("Ball:: hits_paddle")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK_FALSE(ball.hits_block(The_Paddle));
    ball.center.x = The_Paddle.center().x;
    ball.center.y = The_Paddle.center().y;
    CHECK(ball.hits_block(The_Paddle));
    ball.center.x = The_Paddle.center().x + 100;
    CHECK_FALSE(ball.hits_block(The_Paddle));
    ball.center.x = The_Paddle.center().x + 6;
    CHECK(ball.hits_block(The_Paddle));
    ball.center.y = The_Paddle.center().y + 100;
    CHECK_FALSE(ball.hits_block(The_Paddle));
    ball.center.y = The_Paddle.center().y +1;
    CHECK(ball.hits_block(The_Paddle));
}
TEST_CASE("detect collision of ball with top")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {400, 9};
    ball.live = true;
    CHECK(ball.hits_top(config));
    CHECK_FALSE(ball.hits_bottom(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {400, 10};
    ball.live = true;
    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_bottom(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {400, 489};
    ball.live = true;
    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_bottom(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {400, 590};
    ball.live = true;
    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_bottom(config));
}

TEST_CASE("detect collision of ball with corner")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {9, 9};
    ball.live = true;
    CHECK(ball.hits_top(config));
    CHECK(ball.hits_side(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {10, 9};
    ball.live = true;
    CHECK(ball.hits_top(config));
    CHECK_FALSE(ball.hits_side(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {9, 10};
    ball.live = true;
    CHECK_FALSE(ball.hits_top(config));
    CHECK(ball.hits_side(config));

    ball.radius = 10;
    ball.velocity = {0, 0};
    ball.center = {10, 10};
    ball.live = true;
    CHECK_FALSE(ball.hits_top(config));
    CHECK_FALSE(ball.hits_side(config));
}

TEST_CASE("ball doesn't hit blocks")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 3;
    ball.velocity = {2, -20};
    ball.center = {450, 546};
    ball.live = false;
    Block block1 {100, 100, 100, 25};
    Block block2 {400, 422, 100, 100};
    Block block3 {400, 423, 100, 100};
    Block block4 {455, 500, 100, 100};

    CHECK_FALSE(ball.hits_block(block1));
    CHECK_FALSE(ball.hits_block(block2));
    CHECK_FALSE(ball.hits_block(block3));
    CHECK_FALSE(ball.hits_block(block4));
}

TEST_CASE("ball does hit some blocks")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 3;
    ball.velocity = {2, -20};
    ball.center = {450, 546};
    ball.live = false;

    Block block5 {400, 524, 100, 100};
    Block block6 {400, 540, 100, 100};
    Block block7 {456, 500, 100, 100};

    CHECK(ball.hits_block(block5));
    CHECK(ball.hits_block(block6));
    CHECK_FALSE(ball.hits_block(block7));
}

TEST_CASE("check the ball moves correctly")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 3;
    ball.velocity = {2, -20};
    ball.center = {450, 546};
    ball.live = false;

    ball = ball.next(1);
    CHECK(ball.radius == 3);
    CHECK(ball.velocity.width == 2);
    CHECK(ball.velocity.height == -20);
    CHECK(ball.center.x == 452);
    CHECK(ball.center.y == 526);
    CHECK(ball.live == false);
}

TEST_CASE("check the ball moves correctly over a half-step in time")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 3;
    ball.velocity = {2, -20};
    ball.center = {450, 546};
    ball.live = false;
    ball = ball.next(0.5);

    CHECK(ball.radius == 3);
    CHECK(ball.velocity.width == 2);
    CHECK(ball.velocity.height == -20);
    CHECK(ball.center.x == 451);
    CHECK(ball.center.y == 536);
    CHECK(ball.live == false);
}

TEST_CASE("ball doesn't destroy bricks that it shouldn't")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    ball.radius = 3;
    ball.velocity = {2, -20};
    ball.center = {450, 546};
    ball.live = false;

    std::vector<Block> bricks;
    bricks.push_back({100, 100, 100, 25});
    bricks.push_back({200, 100, 100, 25});

    CHECK(ball.destroy_brick(bricks) == false);

    CHECK(ball.radius == 3);
    CHECK(ball.velocity.width == 2);
    CHECK(ball.velocity.height == -20);
    CHECK(ball.center.x == 450);
    CHECK(ball.center.y == 546);
    CHECK(ball.live == false);

    CHECK(bricks == std::vector<Block>{{100, 100, 100, 25}, {200, 100, 100, 25}});
}

TEST_CASE("ball destroys a brick it should")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 3;
    ball.velocity = {2, -20};
    ball.center = {450, 546};
    ball.live = false;
    std::vector<Block> bricks;
    bricks.push_back({400, 540, 100, 100});
    CHECK(ball.destroy_brick(bricks));
    CHECK(bricks.empty());
}

TEST_CASE("destroy a brick and check for the swap")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    ball.radius = 3;
    ball.velocity = {2, -20};
    ball.center = {450, 546};
    ball.live = false;

    std::vector<Block> bricks;
    bricks.push_back({400, 540, 100, 100});
    bricks.push_back({400, 542, 100, 100});
    bricks.push_back({200, 100, 100, 25});
    CHECK(ball.destroy_brick(bricks));
    CHECK(bricks == std::vector<Block>{{200, 100, 100, 25}, {400, 542, 100, 100}});
}

TEST_CASE("Ball:: destroy_brick")
{
    Game_config config;
    Ball ball(The_Paddle, config);
    Block block(20, 20, 20, 20);
    std::vector<Block> bricks;

    ball.center.x = block.center().x + 30;
    ball.center.y = block.center().y + 30;
    CHECK_FALSE(ball.hits_block(block));

    bricks.push_back(block);
    CHECK_FALSE(ball.destroy_brick(bricks));
    CHECK(bricks.size() == 1);

    ball.center.x = block.center().x;
    ball.center.y = block.center().y;
    CHECK(ball.destroy_brick(bricks));
    CHECK(bricks.size() == 0);
}

TEST_CASE("ball equality operators")
{
    Game_config config;
    Ball ball1(The_Paddle, config);
    Ball ball2(The_Paddle, config);

    ball1.center = ge211::Posn<float>(1, 1);
    ball1.radius = 5;
    ball1.live = true;
    ball1.velocity = ge211::Dims<float>(5, 5);

    ball2.center = ball1.center;
    ball2.radius = ball1.radius;
    ball2.live = ball1.live;
    ball2.velocity = ball1.velocity;

    CHECK(ball1 == ball2);

    ball2.center.x += 1;

    CHECK_FALSE(ball1 == ball2);

    ball2.center.x -= 1;
    CHECK(ball1 == ball2);

    ball2.live = false;
    CHECK_FALSE(ball1 == ball2);

    ball2.live = true;
    CHECK(ball1 == ball2);

    ball2.velocity.height *= -1;
    CHECK_FALSE(ball1 == ball2);

    ball2.velocity.height *= -1;
    CHECK(ball1 == ball2);

    ball2.radius += 10;
    CHECK_FALSE(ball1 == ball2);

    ball2.radius -= 10;
    CHECK(ball1 == ball2);
}


TEST_CASE("ball next() function")
{
    Game_config config;
    Ball ball(The_Paddle, config);

    CHECK(ball.hits_block(The_Paddle) == false);
    ball.center.x = 100;
    ball.center.y = 400;
    CHECK(ball.hits_block(The_Paddle));
}


