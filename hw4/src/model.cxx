// YOU DEFINITELY NEED TO MODIFY THIS FILE.

#include "model.hxx"

// Constructs a model from the given `Game_config`. Note
// that the `Game_config` is passed by `const&` but `Model`
// saves its own copy of it.
//
// The `config`, `paddle`, and `ball` member variables need
// to be initialized in an initialization list, not in the body
// of the constructor. And:
//
//   - The paddle needs to be initialized with its top-left
//     at `config.paddle_top_left_0()` and with dimensions
//     `config.paddle_dims`. The *static factory function*
//     `Block::from_top_left(Position, Dimensions)` can be used
//     to construct the `Block` you want.
//
//   - The ball needs to be initialized with the state of the paddle
//     and the game configuration.
//
// Then in the body of the constructor, we need to push_back
// `config.brick_rows * config.brick_cols` into the `bricks`
// vector. In details:
//
//   - Each brick should have dimensions `config.brick_dims()`
//
//   - The first (top-left-most) brick should have its top left
//     at the position {config.side_margin, config.top_margin}.
//
//   - You will need nested loops to create all the bricks in each
//     row and column, but note that the order doesn't matter.
//
//   - The offset between each brick and the next is given by the
//     dimensions of each brick plus `config.brick_spacing`.
//     Or in other words, the x offset is `config.brick_spacing.width
//     + config.brick_dims().width`, and the y offset is the same
//     but with heights.
//
Model::Model(Game_config const& config)
        : config(config),
          paddle(Block::from_top_left(config.paddle_top_left_0(),
                                      config.paddle_dims)),
          ball(paddle, config),
          random_boost_source(-config.max_boost, config.max_boost)
{
    bricks = std::vector<Block>();

   for (int row = 0; row < config.brick_rows; row++) {
       for (int col = 0; col < config.brick_cols; col++) {
               int x = config.side_margin + (config.brick_dims().width + config.brick_spacing.width) * col;
               int y = config.top_margin + (config.brick_dims().height + config.brick_spacing.height) * row ;
               int width = config.brick_dims().width;
               int height = config.brick_dims().height;
               bricks.push_back(ge211::Rect<int>(x, y, width, height));
           }
       }
   }


// Freebie.
void
Model::launch()
{
    ball.live = true;
}

// Warning! Until you write code inside these member functions
// that uses at least one member variable, CLion is likely to
// insist that you can make this function static. Don’t believe
// it! You’ll regret the change if you do once you want to
// access member variables, since a static member function
// doesn't have access to an instance. (You should delete this
// warning after you’ve read it.)
//
// TL;DR: Don't go adding `static` to members.

// If the ball isn't live then you need to make it track the paddle,
// which is best done by constructing a new `Ball` and assigning it to
// `ball`.
void
Model::paddle_to(int x) {

    ge211::Posn<int> top_left(x, paddle.top_left().y);
    ge211::Dims<int> dimensions = config.paddle_dims;
    paddle = Block::from_top_left(top_left, dimensions);

    if (!ball.live) {
        ball.center.x = static_cast<float>(x) + config.paddle_dims.width / 2;
        ball.center.y = paddle.top_left().y - ball.radius;
    }
}

// The description in model.hxx is pretty detailed. What I will add here:
// You probably just want to call `Ball::next(double) const` twice: once
// speculatively as soon as you know that the ball is live, and again at the
// end, storing the result back to the ball that time.
    void
    Model::on_frame(double dt) {
    if (!ball.live) {
        return;
    }

    Ball speculative_ball = ball.next(dt);

    if (speculative_ball.hits_bottom(config)) {
        ball.live = false;
        ball = Ball(paddle, config);
        return;
    }

    if (speculative_ball.hits_top(config)) {
        ball.reflect_vertical();
    }

    if (speculative_ball.hits_side(config)) {
        ball.reflect_horizontal();
    }

    if (speculative_ball.hits_block(paddle)) {
        ball.reflect_vertical();
    }

    if (speculative_ball.destroy_brick(bricks)) {
        ball.reflect_vertical();
        ball.velocity.width += random_boost_source.next();
    }
    ball = ball.next(dt);
}

