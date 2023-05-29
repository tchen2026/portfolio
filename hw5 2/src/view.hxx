#pragma once

#include "model.hxx"

#include <string>

class View
{
public:
    /// View dimensions will use `int` coo
    /// rdinates.
    using Dimensions = ge211::Dims<int>;

    /// View positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// View rectangles will use `int` coordinates.
    using Rectangle = ge211::Rect<int>;

    /// Constructs a view that knows about the given model.
    explicit View(Model const&);

    // We added an argument here so that the
    // controller can communicate UI state (such as a mouse or
    // cursor position):
    void draw(ge211::Sprite_set& set, Position mouse_posn, Position_set position);

    Dimensions initial_window_dimensions() const;

    std::string initial_window_title() const;

    Model::Position screen_to_board(Position physical) const;
    Model::Position board_to_screen(Position logical) const;
    void add_players(ge211::Sprite_set& set, Player player, Position pos) const;

private:
    const Model& model_;
    ge211::Circle_sprite dark_sprite;
    ge211::Circle_sprite light_sprite;
    ge211::Rectangle_sprite board_sprite;
    ge211::Rectangle_sprite board_sprite_g;
    ge211::Rectangle_sprite red_tiles;
    ge211::Rectangle_sprite blue_tiles;
    ge211::Circle_sprite grey_sprite;

};



