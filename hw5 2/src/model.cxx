#include "model.hxx"

using namespace ge211;

Model::Model(int size)
        : Model(size, size)
{ }

Model::Model(int width, int height)
        : board_({width, height})
{
//    turn_ = Player::dark;
    compute_next_moves_();
}


Model::Rectangle
Model::all_positions() const
{
    return board_.all_positions();
}

Player
Model::operator[](Position pos) const
{
    return board_[pos];
}

const Move*
Model::find_move(Position pos) const
{
    auto i = next_moves_.find(pos);

    if (i == next_moves_.end()) {
        // Nothing was found, so return NULL (nullptr in C++)
        return nullptr;
    } else {
        // Dereferences the iterator to get the value then returns a pointer
        // to that value. This is safe as the pointer is to a value living
        // within the `next_moves_` structure.
        return &(*i);
    }
}

void
Model::play_move(Position pos) {
    if (is_game_over()) {
        throw Client_logic_error("Model::play_move: game over");
    }

    const Move *movep = find_move(pos);
    if (movep == nullptr) { // check if there was no such move
        throw Client_logic_error("Model::play_move: no such move");
    }

    really_play_move_(*movep);

//    const Move& move = *movep;
//    board_[move.first] = turn_;
//
//    for (Position p : move.second) {
//        board_[p] = turn_;
//    }
//    bool has_moves = advance_turn_();
//    if (!has_moves) {
//        has_moves = advance_turn_();
//    }
//    if (has_moves) {
//        compute_next_moves_();
//    } else {
//       set_game_over_();
//    }
}

//
// BELOW ARE HELPER FUNCTIONS
// Our tests will run on the public functions, not the helper functions
// However, these will be extremely useful in writing the above functions
//

Position_set
Model::find_flips_(Position current, Dimensions dir) const {
    std::vector<Position> flips;
    Player current_player = turn_;
    Player opponent = other_player(current_player);
    current += dir;
    while (board_.good_position(current)) {
        if (board_[current] == opponent) {
            flips.push_back(current);
            current += dir;
        } else if (board_[current] == current_player) {
            return Position_set(flips.begin(), flips.end());
        } else {
            break;
        }
    }
    return Position_set();
}

Position_set
Model::evaluate_position_(Position pos) const {

    std::vector<Position> moves;
    if (board_[pos] != Player::neither) {
        return Position_set();
    }

    for (Dimensions dir: board_.all_directions()) {
        Position_set flips = find_flips_(pos, dir);
        moves.insert(moves.end(), flips.begin(), flips.end());
    }
    if (!moves.empty()) {
        moves.push_back(pos);
    }
//    return p;
    return Position_set(moves.begin(), moves.end());
}

void
Model::compute_next_moves_() {
    next_moves_.clear();
    bool in_opening_phase = false;

    for (Position pos: board_.center_positions()) {
        if (board_[pos] == Player::neither) {
            next_moves_[pos] = {pos};
            in_opening_phase = true;
        }
    }
    if (in_opening_phase) {
        return;
    }
    for (Position pos: all_positions()) {
        Position_set moves = evaluate_position_(pos);
        if (!moves.empty()) {
            next_moves_[pos] = moves;
        }
    }
}

bool
Model::advance_turn_()
{
    turn_ = other_player(turn_);
    compute_next_moves_();
    return !next_moves_.empty();
}

void
Model::set_game_over_()
{
    int dark_count = board_.count_player(Player::dark);
    int light_count = board_.count_player(Player::light);
    turn_ = Player::neither;
    if (dark_count > light_count) {
        winner_ = Player::dark;
    } else if (dark_count < light_count) {
        winner_ = Player::light;
    } else {
        winner_ = Player::neither;
    }
//    if (winner_ == Player::neither) {
//        turn_ = Player::neither;
//    } else {
//        turn_ = other_player(winner_);
//    }
}

void
Model::really_play_move_(Move move)
{
//    board_[move.first] = turn_;
//    for (Position pos : move.second) {
//        board_[pos] = turn_;
//    }
    board_.set_all(move.second, turn_);
    bool has_moves = advance_turn_();
    if (!has_moves) {
        has_moves = advance_turn_();
        if (!has_moves) {
            set_game_over_();
        }
    }
}

