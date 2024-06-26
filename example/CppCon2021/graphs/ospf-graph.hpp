//
//  Authors
//  Copyright
//  License
//  No Warranty Disclaimer
//
#pragma once

// Data and graph from OSPF example in Boost Graph Library book

#include <vector>
#include <string>
#include <tuple>

// clang-format off

std::vector<std::string> ospf_vertices {
  "RT1",
  "RT2",
  "RT3",
  "RT4",
  "RT5",
  "RT6",
  "RT7",
  "RT8",
  "RT9",
  "RT10",
  "RT11",
  "RT12",
  "N1",
  "N2",
  "N3",
  "N4",
  "N6",
  "N7",
  "N8",
  "N9",
  "N10",
  "N11",
  "N12",
 "N13",
 "N14",
 "N15",
 "H1",
};

std::vector<std::tuple<std::string, std::string, size_t>> ospf_edges {
  {"RT1", "N1", 3},
  {"RT1", "N3", 1},
  {"RT2", "N2", 3},
  {"RT2", "N3", 1},
  {"RT3", "RT6", 8},
  {"RT3", "N3", 1},
  {"RT3", "N4", 2},
  {"RT4", "N3", 1},
  {"RT4", "RT5", 8},
  {"RT5", "RT4", 8},
  {"RT5", "RT6", 7},
  {"RT5", "RT7", 6},
  {"RT5", "N12", 8},
  {"RT5", "N13", 8},
  {"RT5", "N14", 8},
  {"RT6", "RT3", 6},
  {"RT6", "RT5", 6},
  {"RT6", "RT10", 7},
  {"RT7", "RT5", 6},
  {"RT7", "N6", 1},
  {"RT7", "N12", 2},
  {"RT7", "N15", 9},
  {"RT8", "N6", 1},
  {"RT8", "N7", 4},
  {"RT9", "N9", 1},
  {"RT9", "N11", 3},
  {"RT10", "RT6", 5},
  {"RT10", "N6", 1},
  {"RT10", "N8", 3},
  {"RT11", "N8", 2},
  {"RT11", "N9", 1},
  {"RT12", "N9", 1},
  {"RT12", "N10", 2},
  {"RT12", "H1", 10},
  {"N3", "RT1", 0},
  {"N3", "RT2", 0},
  {"N3", "RT3", 0},
  {"N3", "RT4", 0},
  {"N6", "RT7", 0},
  {"N6", "RT8", 0},
  {"N6", "RT10", 0},
  {"N8", "RT10", 0},
  {"N8", "RT11", 0},
  {"N9", "RT9", 0},
  {"N9", "RT11", 0},
  {"N9", "RT12", 0},
  };

std::vector<std::tuple<size_t, size_t, size_t>> ospf_index_edge_list {
{     0,    12,     3},
{     0,    14,     1},
{     1,    13,     3},
{     1,    14,     1},
{     2,     5,     8},
{     2,    14,     1},
{     2,    15,     2},
{     3,    14,     1},
{     3,     4,     8},
{     4,     3,     8},
{     4,     5,     7},
{     4,     6,     6},
{     4,    22,     8},
{     4,    23,     8},
{     4,    24,     8},
{     5,     2,     6},
{     5,     4,     6},
{     5,     9,     7},
{     6,     4,     6},
{     6,    16,     1},
{     6,    22,     2},
{     6,    25,     9},
{     7,    16,     1},
{     7,    17,     4},
{     8,    19,     1},
{     8,    21,     3},
{     9,     5,     5},
{     9,    16,     1},
{     9,    18,     3},
{    10,    18,     2},
{    10,    19,     1},
{    11,    19,     1},
{    11,    20,     2},
{    11,    26,    10},
{    14,     0,     0},
{    14,     1,     0},
{    14,     2,     0},
{    14,     3,     0},
{    16,     6,     0},
{    16,     7,     0},
{    16,     9,     0},
{    18,     9,     0},
{    18,    10,     0},
{    19,     8,     0},
{    19,    10,     0},
{    19,    11,     0},
};

std::vector<std::vector<std::tuple<size_t, size_t>>> ospf_index_adjacency_list {
  /*  0 */ { {  12,  3 },{  14,  1 },},
  /*  1 */ { {  13,  3 },{  14,  1 },},
  /*  2 */ { {   5,  8 },{  14,  1 },{  15,  2 },},
  /*  3 */ { {  14,  1 },{   4,  8 },},
  /*  4 */ { {   3,  8 },{   5,  7 },{   6,  6 },{  22,  8 },{  23,  8 },{  24,  8 },},
  /*  5 */ { {   2,  6 },{   4,  6 },{   9,  7 },},
  /*  6 */ { {   4,  6 },{  16,  1 },{  22,  2 },{  25,  9 },},
  /*  7 */ { {  16,  1 },{  17,  4 },},
  /*  8 */ { {  19,  1 },{  21,  3 },},
  /*  9 */ { {   5,  5 },{  16,  1 },{  18,  3 },},
  /* 10 */ { {  18,  2 },{  19,  1 },},
  /* 11 */ { {  19,  1 },{  20,  2 },{  26,  10 },},
  /* 12 */ { },
  /* 13 */ { },
  /* 14 */ { {   0,  0 },{   1,  0 },{   2,  0 },{   3,  0 },},
  /* 15 */ { },
  /* 16 */ { {   6,  0 },{   7,  0 },{   9,  0 },},
  /* 17 */ { },
  /* 18 */ { {   9,  0 },{  10,  0 },},
  /* 19 */ { {   8,  0 },{  10,  0 },{  11,  0 },},
  /* 20 */ { },
  /* 21 */ { },
  /* 22 */ { },
  /* 23 */ { },
  /* 24 */ { },
  /* 25 */ { },
  /* 26 */ { },
};

std::vector<std::tuple<std::string, size_t>> ospf_shortest_path_distances {
  {  "RT1",  7 },
  {  "RT2",  7 },
  {  "RT3",  6 },
  {  "RT4",  7 },
  {  "RT5",  6 },
  {  "RT6",  0 },
  {  "RT7",  8 },
  {  "RT8",  8 },
  {  "RT9", 11 },
  { "RT10",  7 },
  { "RT11", 10 },
  { "RT12", 11 },
  {   "N1", 10 },
  {   "N2", 10 },
  {   "N3", 7  },
  {   "N4",  8 },
  {   "N6",  8 },
  {   "N7", 12 },
  {   "N8", 10 },
  {   "N9", 11 },
  {  "N10", 13 },
  {  "N11", 14 },
  {  "N12", 10 },
  {  "N13", 14 },
  {  "N14", 14 },
  {  "N15", 17 },
  {   "H1", 21 },
};

// clang-format on
