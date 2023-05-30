#include "model.hxx"
#include <catch.hxx>

// We'll use this same game config throughout.
Game_config const config;

TEST_CASE("ball moving freely")
{
    Model m(config);

    // Get rid of all the bricks and make the ball move slowly upward so
    // we don't hit anything:
    m.bricks.clear();
    m.ball.velocity = {8, -24};
    m.ball.live = true;

    // Make a copy of the ball's position, which we'll use to compute
    // where we expect the ball to go.
    Position expected_center = m.ball.center;

    // If the model advances by 0.5 s then we expect the center to move
    // accordingly:
    m.on_frame(0.5);
    expected_center += 0.5 * m.ball.velocity;
    CHECK(m.ball.center == expected_center);

    // Again, but only 0.25 s this time.
    m.on_frame(0.25);
    expected_center += 0.25 * m.ball.velocity;
    CHECK(m.ball.center == expected_center);

    // Again, but 0.125 s this time.
    m.on_frame(0.125);
    expected_center += 0.125 * m.ball.velocity;
    CHECK(m.ball.center == expected_center);
}

TEST_CASE("destroy one brick")
{
    Model m(config);

    // Let's run at 1 fps.
    double const dt = 1;

    // We're going to stub the model's random number generator so it
    // always returns 14:
    float const boost = 14;

    // Stub it:
    m.random_boost_source.stub_with(boost);

    // Make there be only one brick:
    m.bricks.clear();
    m.bricks.push_back({250, 200, 100, 20});

    // The ball is headed toward the brick and should hit it in
    // the 4th frame if we simulate 1 frame per second:
    m.ball.live = true;
    m.ball.center = {300, 400};
    m.ball.velocity = {0, -50};

    // Make a copy of the ball, which we'll use to computer what we
    // expect the ball to do:
    Ball expected_ball(m.ball);

    // Simulate the passage of 1 frame (1 s):
    m.on_frame(dt);
    expected_ball = expected_ball.next(dt);
    CHECK(m.ball == expected_ball);
    CHECK(m.bricks.size() == 1);

    // Simulate the passage of another frame:
    m.on_frame(dt);
    expected_ball = expected_ball.next(dt);
    CHECK(m.ball == expected_ball);
    CHECK(m.bricks.size() == 1);

    // Simulate the passage of another frame:
    m.on_frame(dt);
    expected_ball = expected_ball.next(dt);
    CHECK(m.ball == expected_ball);
    CHECK(m.bricks.size() == 1);

    // Simulate the passage of another frame, in which the ball
    // destroys a brick. The model should reflect and boost the ball
    // before moving it, so we need to do the same with our expected
    // ball.
    m.on_frame(dt);
    expected_ball.velocity.height *= -1;
    expected_ball.velocity.width += boost;
    expected_ball = expected_ball.next(dt);
    CHECK(m.ball == expected_ball);
    CHECK(m.bricks.empty());
}

TEST_CASE("bounce off paddle")
{
    Model m(config);

    // No bricks, please.
    m.bricks.clear();

    // Let's run at 8 fps.
    double const dt = 0.125;

    // The ball is headed toward the paddle at 50 px/s from 75 px away,
    // so it should it it in 1.5 s. If we simulate 8 fps then it should
    // hit in the 12th frame.
    m.paddle = {50, 400, 600, 50};
    m.ball.live = true;
    m.ball.velocity = {0, 50};
    m.ball.center.x = 300;
    m.ball.center.y = m.paddle.y - 75 - m.ball.radius / 2;

    // Make a copy of the ball for computing what we expect it to do.
    Ball expected_ball(m.ball);

    // Simulate 11 frames (1.375 s).
    for (int i = 0; i < 11; ++i) {
        // run both the expected ball and the model for a frame
        // then check that they agree still
        expected_ball = expected_ball.next(dt);
        m.on_frame(dt);
        CHECK(m.ball == expected_ball);
    }

    // Simulate one frame in which the ball finally hits the paddle,
    // which means that it reflects vertically.
    expected_ball.velocity.height *= -1;
    expected_ball = expected_ball.next(dt);
    m.on_frame(dt);
    CHECK(m.ball == expected_ball);

    // Simulate 8 more frames (1 s), now with the ball headed away from
    // the paddle.
    for (int i = 0; i < 8; ++i) {
        expected_ball = expected_ball.next(dt);
        m.on_frame(dt);
        CHECK(m.ball == expected_ball);
    }
}
//
// "WRITE MORE TESTS!!!" is never bad advice
//

TEST_CASE("ball falls off the the bottom of the screen")
{
    Model model(config);
    float ball_x = 300.0f;
    float ball_y = static_cast<float>(config.scene_dims.height) - static_cast<float>(model.ball.radius) - 1.0f;
    model.ball.center = {ball_x, ball_y};
    model.ball.live = true;

    int max_iterations = 1000;
    int iterations = 0;
    while (model.ball.live && iterations < max_iterations) {
        model.on_frame(1.0);
        iterations++;
    }
    CHECK_FALSE(model.ball.live);
}

TEST_CASE("paddle moves correctly")
{
    Game_config config;
    Model model(config);

    int new_x = 100;

    model.paddle_to(new_x);
    CHECK(model.paddle.top_left().x == new_x);
    CHECK(model.ball.live == false);
}

TEST_CASE("Ball is launched")
{
    Game_config config;
    Model model(config);
    model.launch();
    CHECK(model.ball.live == true);
}

TEST_CASE("Ball hits top of screen")
{
    Game_config config;
    Model model(config);
    model.ball.live = true;
    model.ball.center = {static_cast<float>(config.scene_dims.width) / 2,
                         static_cast<float>(config.ball_radius) + 1};
    model.ball.velocity.width = 0;
    model.ball.velocity.height = -50.0f;

    double dt = 1.0;
    model.on_frame(dt);
    CHECK(model.ball.velocity.width == 0);
    CHECK(model.ball.velocity.height == 50.0f);
}

TEST_CASE("ball hits sides of the screen")
{
    Game_config config;
    Model model(config);
    model.ball.live = true;
    model.ball.center = {static_cast<float>(config.ball_radius + 1), static_cast<float>(config.scene_dims.height / 2)};
    model.ball.velocity.width = -50.0f;
    model.ball.velocity.height = 0;

    double dt = 1.0;
    model.on_frame(dt);
    CHECK(model.ball.velocity.width == 50.0f);
    CHECK(model.ball.velocity.height == 0);

    model.ball.live = true;
    model.ball.center = {static_cast<float>(config.scene_dims.width - config.ball_radius -1), static_cast<float>(config.scene_dims.height / 2)};
    model.ball.velocity.width = 50.0f;
    model.ball.velocity.height = 0;

    model.on_frame(dt);
    CHECK(model.ball.velocity.width == -50.0f);
    CHECK(model.ball.velocity.height == 0);
}

TEST_CASE("ball hits the paddle")
{
    Game_config config;
    Model model(config);
    model.ball.live = true;
    model.ball.center = {static_cast<float>(config.scene_dims.width) / 2, static_cast<float>(config.scene_dims.height) - config.paddle_dims.height - config.ball_radius - 1};
    model.ball.velocity.width = 0;
    model.ball.velocity.height = -50.0f;

    double dt = 1.0;
    model.on_frame(dt);
    CHECK(model.ball.velocity.width == 0);
    CHECK(model.ball.velocity.height == -50.0f);
}


TEST_CASE("destroy two bricks without boosts")
{
    Model model(config);
    model.bricks.clear();
    model.paddle_to(0);
    model.random_boost_source.stub_with({0});
    model.bricks.push_back({250, 100, 100, 100});
    model.bricks.push_back({250, 300, 100, 100});
    model.ball.velocity = {0, -80};
    model.ball.live = true;
    model.ball.center = {300, 210};
    model.ball.radius = {3};

    model.on_frame(0.5);
    CHECK(model.ball.velocity.height== 80);
    CHECK(model.ball.velocity.width== 0);
    CHECK(model.ball.live == true);
    CHECK(model.ball.center.x == 300);
    CHECK(model.ball.center.y == 250);
    CHECK(model.ball.radius == 3);
    CHECK(model.bricks == std::vector<Block>{{250, 300, 100, 100}});

    model.on_frame(0.5);
    CHECK(model.ball.live == true);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.height == 80);
    CHECK(model.ball.velocity.width== 0);
    CHECK(model.ball.center.x == 300);
    CHECK(model.ball.center.y == 290);
    CHECK(model.bricks == std::vector<Block>{{250, 300, 100, 100}});

    model.on_frame(0.5);
    CHECK(model.ball.live == true);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == 0);
    CHECK(model.ball.velocity.height == -80);
    CHECK(model.ball.center.x == 300);
    CHECK(model.ball.center.y == 250);
    CHECK(model.bricks.empty());
}
TEST_CASE ("destroy two bricks with boosts")
{
    Model model(config);
    model.bricks.clear();
    model.paddle_to(0);
    model.random_boost_source.stub_with({-12, 14});
    model.bricks.push_back({250, 100, 100, 100});
    model.bricks.push_back({250, 300, 100, 100});
    model.ball.radius = {3};
    model.ball.velocity = {8, -80};
    model.ball.center = {300, 210};
    model.ball.live = true;

    model.on_frame(0.5);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == -4);
    CHECK(model.ball.velocity.height == 80);
    CHECK(model.ball.center.x == 298 );
    CHECK(model.ball.center.y == 250);
    CHECK(model.ball.live == true);
    CHECK(model.bricks == std::vector<Block>{{250, 300, 100, 100}});

    model.on_frame(0.5);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == -4);
    CHECK(model.ball.velocity.height == 80);
    CHECK(model.ball.center.x == 296);
    CHECK(model.ball.center.y == 290);
    CHECK(model.ball.live == true);
    CHECK(model.bricks == std::vector<Block>{{250, 300, 100, 100}});

    model.on_frame(0.5);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == 10);
    CHECK(model.ball.velocity.height == -80);
    CHECK(model.ball.center.x == 301);
    CHECK(model.ball.center.y == 250);
    CHECK(model.ball.live == true);
    CHECK(model.bricks.empty());
}

TEST_CASE("destroy only brick with boost")
{
    Model model(config);
    model.bricks.clear();
    model.paddle_to(0);
    model.bricks.push_back({350, 100, 100, 100});
    model.random_boost_source.stub_with({-4, 20, 20, 20, 20});
    model.ball.radius = {3};
    model.ball.velocity = {3, -10};
    model.ball.center = {403, 205};
    model.ball.live = true;

    model.on_frame(1);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == -1);
    CHECK(model.ball.velocity.height == 10);
    CHECK(model.ball.center.x == 402);
    CHECK(model.ball.center.y == 215);
    CHECK(model.ball.live == true);
    CHECK(model.bricks.empty());
}

TEST_CASE("destroy only brick, without boost")
{
    Model model(config);
    model.bricks.clear();
    model.paddle_to(0);
    model.bricks.push_back({350, 100, 100, 100});
    model.random_boost_source.stub_with({0});
    model.ball.radius = {3};
    model.ball.velocity = {3, -10};
    model.ball.center = {403, 205};
    model.ball.live = true;

    model.on_frame(1);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == 3);
    CHECK(model.ball.velocity.height == 10);
    CHECK(model.ball.center.x == 406);
    CHECK(model.ball.center.y == 215);
    CHECK(model.ball.live == true);
    CHECK(model.bricks.empty());
}

TEST_CASE("miss only one brick")
{
    Model model(config);
    model.bricks.clear();
    model.paddle_to(0);
    model.bricks.push_back({350, 100, 100, 100});
    model.ball.radius = {3};
    model.ball.velocity = {3, -10};
    model.ball.center = {400, 215};
    model.ball.live = true;

    model.on_frame(1);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == 3);
    CHECK(model.ball.velocity.height == -10);
    CHECK(model.ball.center.x == 403);
    CHECK(model.ball.center.y == 205);
    CHECK(model.ball.live == true);
    CHECK(model.bricks == std::vector<Block>{{350, 100, 100, 100}});
}

//TEST_CASE("reflect off paddle on angle")
//{
    //Model model(config);
    //model.bricks.clear();
    //model.paddle_to(0);
    //model.paddle_to(200);
    //model.ball.radius = {3};
    //model.ball.velocity = {6, 10};
    //model.ball.center = {250, 545};
    //model.ball.live = true;

    //model.on_frame(0.5);
    //CHECK(model.ball.radius == 3);
    //CHECK(model.ball.velocity.width == 6);
    //CHECK(model.ball.velocity.height == -10);
    //CHECK(model.ball.center.x == 253);
    //CHECK(model.ball.center.y == 540);
    //CHECK(model.ball.live == true);
//}

TEST_CASE("ball doesn't hit bottom and doesn't die")
{
    Model model(config);
    model.bricks.clear();
    model.paddle_to(0);
    model.ball.radius = 3;
    model.ball.velocity = {8, 76};
    model.ball.center = {400, 570};
    model.ball.live = true;

    model.on_frame(0.25);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == 8);
    CHECK(model.ball.velocity.height == 76);
    CHECK(model.ball.center.x == 402);
    CHECK(model.ball.center.y == 589);
    CHECK(model.ball.live == true);
}

TEST_CASE("ball hits side but not bottom and doesn't die")
{
    Game_config cfg;
    cfg.scene_dims.width = 800;
    Model model(cfg);
    model.bricks.clear();
    model.paddle_to(0);
    model.ball.radius = 3;
    model.ball.velocity = {60, 76};
    model.ball.center = {790, 570};
    model.ball.live = true;

    model.on_frame(0.25);
    CHECK(model.ball.radius == 3);
    CHECK(model.ball.velocity.width == -60);
    CHECK(model.ball.velocity.height == 76);
    CHECK(model.ball.center.x == 775);
    CHECK(model.ball.center.y == 589);
    CHECK(model.ball.live == true);

}

TEST_CASE("move paddle with dead ball")
{
    Game_config config;
    Model model(config);

    int paddleX = 400;
    model.paddle_to(paddleX);

    CHECK(model.ball.live == false);
    CHECK(model.ball.center.x == static_cast<float>(paddleX) + config.paddle_dims.width / 2);
    CHECK(model.ball.center.y == model.paddle.top_left().y - model.ball.radius);
}

TEST_CASE("hits bottom")
{
    Model model(config);
    model.bricks.clear();
    double const dt = 0.125;

    model.ball.live = true;
    model.ball.velocity = {0, 50};
    model.ball.center.x = config.scene_dims.width / 2;
    model.ball.center.y = config.scene_dims.height - 1;

    Ball expected_ball(model.ball);
    CHECK(model.ball.live == true);
    model.on_frame(dt);
    CHECK(model.ball.live == false);
}

TEST_CASE("move paddle with ball")
{
    Model model(config);

    model.ball.center.x = 1;
    model.paddle_to(config.scene_dims.width / 2);
    CHECK(model.ball.center.x == model.paddle.center().x);

    model.ball.center.x = 1;
    model.ball.center.y = config.scene_dims.height / 2;
    model.ball.live = true;
    model.paddle_to(10);
    CHECK_FALSE(model.ball.center.x == model.paddle.center().x);
    CHECK(model.ball.center.x == 1);

    model.ball.live = false;
    model.paddle_to(12);
    CHECK(model.ball.center.x == model.paddle.center().x);
}

TEST_CASE("ball bounces off top and side of screen")
{
    Model model(config);
    model.bricks.clear();
    double const dt = 0.125;

    model.ball.live = true;
    model.ball.velocity = {50, -50};
    model.ball.center.x = config.scene_dims.width - 1;
    model.ball.center.y = 1;

    Ball expected_ball(model.ball);
    CHECK(model.ball.live == true);
    model.on_frame(dt);
    expected_ball.velocity.width *= -1;
    expected_ball.velocity.height *= -1;
    expected_ball = expected_ball.next(dt);
    CHECK(model.ball == expected_ball);
}

TEST_CASE("move paddle")
{
    Model model(config);
    model.ball.center.x = 1;
    model.ball.live = false;
    model.paddle_to(config.scene_dims.width / 2);
    CHECK(model.ball.center.x == model.paddle.center().x);

    model.ball.center.x = 1;
    model.ball.center.y = config.scene_dims.height/2;
    model.ball.live = true;
    model.paddle_to(10);
    CHECK_FALSE(model.ball.center.x == model.paddle.center().x);
    CHECK(model.ball.center.x == 1);
}
