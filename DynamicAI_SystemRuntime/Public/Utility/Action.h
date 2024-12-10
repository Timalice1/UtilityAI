#pragma once

#include "GameplayTags.h"
#include "CoreMinimal.h"
#include "Utility/Scorer.h"
#include "Action.generated.h"

// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⣛⡍⠙⢿⣿⠟⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠟⣩⣥⣬⣭⣭⣍⣀⠲⠤⠄⠙⠈⡟⠄⠿⣛⣛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⣫⢀⣬⣍⣓⡚⢛⣛⣛⣛⣛⠛⠿⠿⠷⠄⠀⠀⣬⣶⠿⢛⣫⡉⠄⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⡛⣡⣾⠃⣿⣿⣿⣿⣿⣿⣏⣉⡳⠂⢉⣠⡰⢾⡷⠶⠒⠈⢐⠢⣭⡙⠻⣦⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢁⠊⣀⠌⢠⣶⣶⣀⣿⣿⢿⣿⣿⠿⠿⠝⠁⠀⢥⣬⣭⣭⣭⣄⡀⠠⠘⢿⡟⠝⢄⣦⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢿⡀⠠⠍⡀⠈⡿⠛⠟⠻⠷⡀⠈⠛⠈⠄⢬⣭⣄⣒⡈⢻⣭⣍⡉⠙⠓⠄⠰⡙⢷⡀⠹⣧⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⡩⠖⣈⡱⠦⠀⠀⠃⢃⠀⠠⠂⠄⡐⡀⣡⠳⣦⡌⢪⣿⣦⣍⠉⣛⡻⢟⠻⢷⣤⠀⠀⠘⡌⠳⡄⢹⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⣨⣶⡿⠁⣠⣭⢀⠔⢀⢁⡔⡠⢰⣶⡇⢿⣮⡁⢹⡛⠄⠉⠛⠧⠍⠀⠉⢂⣀⠐⣍⠳⠸⡆⠈⠀⠹⣇⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⢃⣿⠀⡜⠘⣴⠇⣿⠏⡿⠀⠹⣿⣏⠻⣦⠘⠗⠀⠀⠀⠐⠀⠙⣧⠈⢳⡄⢻⣀⠹⣠⠹⡸⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⣿⣿⣿⣿⣿⣿⣿⣿⢸⢃⠆⣷⡜⣹⢸⠏⠄⠃⠀⠂⠈⠙⠆⠀⠠⠐⣶⣧⠀⣰⣼⣿⠈⠂⢀⠁⢆⠈⠆⠘⢷⡅⢿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⡁⣠⣶⣿⣿⣿⣿⣿⣿⣿⣿⠈⠎⠂⢿⡇⠹⡌⠀⠸⠈⢀⣩⣀⣈⡙⠦⠹⣖⢨⣿⣿⣿⣿⣿⢐⣀⣌⠠⣄⠂⠻⡘⣷⣌⠈⢿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⢸⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠀⠀⠈⠇⠀⢿⠀⠺⣤⣸⣿⣿⣿⣿⡷⣦⣽⣿⣿⣿⣿⣿⠇⠀⣿⡟⠣⡙⣧⡀⠑⠈⢿⣿⣦⡙⢿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⡐⠸⣦⡻⠿⣿⣿⣿⡿⠿⢟⠋⠁⠔⣊⣤⠀⠄⣀⠈⠆⠁⠈⢟⢿⣿⣿⣿⣄⣙⣿⠿⣫⣿⣿⠋⠀⠇⠘⢿⣌⠢⣌⡋⢦⡡⠰⣝⢿⣿⣆⢻⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⡿⠿⠿⠷⠄⠛⠿⣷⣶⣄⣀⡀⠅⠀⢊⠉⡉⠙⣿⣧⠀⡈⠁⡀⠀⢴⡦⠑⠻⢿⣿⣿⣶⣷⣿⣿⡿⡡⠀⠃⠀⠀⠀⠹⢿⣦⡙⠢⠑⣤⡈⢦⡹⣿⠈⣿⣿⣿⣿
// ⣿⣿⣿⡟⢋⡠⠔⣛⣩⣭⣤⡤⠍⠀⠀⠀⣠⣴⡎⠀⠋⠈⠀⠆⡼⠿⠧⣼⡖⡀⠔⠚⠀⣘⡀⠀⠨⢉⠛⠻⠿⠋⠈⠄⠀⠀⣄⣧⠰⣤⡀⠙⠿⣎⡛⠶⢮⣤⣑⡀⠆⣿⣿⣿⣿
// ⣿⡟⡩⣊⣵⣾⡿⢛⣩⠍⠀⠀⠠⠄⠀⠴⣿⣿⣷⡀⠀⢀⣠⠊⠀⠀⠀⠈⢰⣷⡀⢀⠈⠛⠃⡀⠀⠀⡉⠐⠒⠊⠁⡀⠀⣀⡙⢿⡆⣿⣷⠰⣶⣶⡄⠉⠐⢮⣙⠻⣷⣌⠻⣿⣿
// ⣿⣆⣾⣿⣿⠡⡾⢁⡂⠐⠢⠤⠤⠐⠒⠂⠀⠈⠉⠛⠛⠋⠀⠀⠀⠀⠀⠀⢿⠿⠃⠿⣷⣦⣤⣈⠁⢀⡀⢠⠀⣠⣾⠷⠚⢋⣁⣈⡁⢻⣿⠀⣿⣿⣷⡸⣧⠀⠛⠿⣮⡙⢧⡘⣿
// ⣻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡀⢀⣤⠄⡰⢂⠐⢻⣧⠰⠀⢰⣷⣦⣬⣉⠛⠿⣿⣦⡀⠀⢸⡿⢀⣴⣾⣿⡿⠿⠇⢸⣿⠠⠿⣿⣿⣿⣦⣼⠀⢱⣮⠛⢦⠳⢸
// ⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⢚⠩⢐⣩⡴⠞⢡⠃⠀⣿⣿⡆⠀⠈⣿⣿⠿⢿⣿⣶⣄⠙⣿⡀⠀⢀⣾⠟⢉⣠⣴⣶⣆⠹⠇⠀⠀⣿⠿⢛⠛⢿⠀⣸⣿⡇⣌⢣⢸
// ⣿⣿⣿⣿⣿⣿⣿⣿⠷⠀⠀⠀⠀⠀⠀⠰⡀⠨⣭⣵⣶⡇⣾⢀⢰⣿⡿⠀⢀⠾⣛⡉⠀⠂⢹⣿⣿⣧⡈⢷⣶⠞⣁⣴⣿⣿⠟⠹⡟⠀⠀⠀⢠⡿⠈⣭⡅⠘⢰⣿⣿⡇⣿⡆⠆
// ⣿⣿⣿⣿⣿⣿⢟⡕⣠⢂⣦⡉⠁⠀⠀⠀⠈⢄⠙⢿⣿⣿⡜⠸⢸⡿⢡⠇⣰⣾⣿⣿⢸⡄⢿⣿⣿⠁⠉⢠⠀⠚⢻⣿⣿⡟⠸⠃⢉⠀⠀⣴⠇⠁⠈⠥⠂⢠⣿⣿⣿⠰⣿⢃⠀
// ⣿⣿⣿⣿⡿⣱⢋⠜⠁⣠⠞⣡⡆⢀⠀⡀⠀⠀⠀⠈⠻⣿⣷⣄⢆⠃⣿⠀⢻⣿⣿⣿⣶⣄⠈⢿⣿⣦⣀⣸⠀⣀⣸⣿⣿⡟⢀⣴⡸⡆⠠⡈⣔⠀⠠⢀⣴⣿⣿⠟⠁⡡⢣⣾⢀
// ⣿⣿⣿⣿⢃⢃⠊⣰⢰⠏⣼⣿⡁⢸⣷⣄⠀⠑⢶⣆⠀⠘⠻⢿⣦⠸⣿⣇⠀⠻⢿⣿⣿⣿⣷⡶⠘⢋⣿⣟⢻⠿⢛⣛⣋⣴⣾⣿⣷⡄⠀⠁⠛⢀⣴⣿⣿⣿⠶⠂⣀⣴⣿⠏⣼
// ⣿⣿⣿⣿⠀⠂⣴⣿⢸⢸⣿⣿⣧⠈⡟⠉⡑⠄⠀⠙⣷⣤⣦⣆⠈⣧⡙⠛⠀⠌⠒⠮⠭⠭⠅⠀⡀⠿⠿⠿⠀⠘⠿⠿⠿⢛⣋⣭⠉⠀⠰⠁⢀⣾⣿⣿⣿⣶⣶⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⡄⢰⣿⣿⡌⠸⣿⣿⣿⣷⣷⠀⠁⠀⠀⠑⠀⠙⣿⣿⡀⠉⢔⠀⠀⠀⠀⠀⢆⠐⠵⣾⣶⣶⣶⣶⡦⠑⠈⠉⢛⠛⠉⠀⠀⠠⠁⡀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣇⠀⠙⢿⣿⣦⣙⣿⣿⡿⠃⣀⠂⠀⠐⠈⠃⢈⡁⠄⠁⠀⣡⡀⡄⢈⣄⠀⠰⢦⡢⠄⢁⢩⣭⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠱⣜⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⠋⠴⠶⠖⠀⠀⡀⠠⠈⠁⠐⢠⡎⢸⣿⣧⠀⣜⢿⠷⠄⠀⠀⠀⠈⠈⠉⠀⠀⠠⣴⢀⠆⠀⠀⠀⠀⡠⠰⣅⣀⣉⣒⣈⣽⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⡆⠑⠁⠀⢀⡴⠉⡇⢸⣿⣿⣷⡈⢮⣷⡄⠠⣤⣤⣀⣴⣦⣬⠒⠀⠆⠊⠀⠀⠀⠀⠀⣷⣄⠈⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣦⡠⠤⢒⣉⡁⢀⣼⣿⣿⡇⡆⠁⠸⣿⣿⣿⣿⣄⠠⠀⢰⣿⣿⣿⣿⡏⠀⠀⡀⢸⠀⠀⠀⠀⠠⢰⣿⣿⣿⣶⣦⣬⣭⣽⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⢠⣿⣿⣿⡏⠀⡇⠀⡀⢿⣿⣿⣿⣿⡇⠀⠜⣿⣿⣿⡟⠀⢠⠈⠁⠀⡀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⣾⣿⣿⣿⣧⢀⢀⡀⠰⠘⣿⣿⣿⡿⠁⠀⣀⣿⡿⠋⠀⠀⡀⢆⢸⠀⣷⠀⠀⢠⠁⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣿⣿⡌⡘⣷⡀⠀⣿⣿⡿⠃⠀⣪⣾⠟⠁⢀⠄⣰⣿⡆⠆⠀⠹⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠁⠀⠈⣿⣿⣿⣿⣷⠰⡘⣿⡄⠘⣉⠴⣀⣴⠿⠁⠠⠀⣡⣾⣿⣿⣿⡐⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠸⣿⣿⣿⣿⣇⢣⠹⣷⡈⠀⣿⡟⠁⠀⣠⡄⢰⣿⣿⣿⣿⣿⣧⠀⠀⣰⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢄⠀⠀⠙⢿⣿⣿⣿⡘⣧⠹⣷⡈⣭⣤⡄⢀⣻⣧⠘⣿⣿⣿⣿⣿⡏⠈⠆⣿⣦⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠈⠢⠀⠀⠈⠻⣿⣿⣷⡸⣇⢻⣷⠸⣿⣿⡄⠹⣿⣧⠸⣿⣿⠟⠁⠂⡆⢠⣿⡏⠈⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠁⡄⣦⡀⠀⢸⣿⣿⣷⡘⡌⣿⡇⢿⣿⣿⡄⠹⣿⡄⢻⡟⠀⠐⠂⠬⠘⠛⣁⡆⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠃⢸⣿⣦⠀⣿⣿⣿⣷⡰⡘⣿⠸⣿⣿⣿⡄⠹⣿⡈⣧⠀⠑⢄⠀⣾⣿⡿⠐⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⠀⠘⢿⣿⣿⣿⣿⣿⣿⣧⢁⢻⡇⣿⣿⣿⣷⡄⠹⣷⡈⠈⠁⠠⠄⠉⠀⢀⣤⡀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡌⠸⣧⢹⣿⣿⣿⣿⣄⠉⡅⢰⡆⢠⣿⣶⣾⣿⣿⣿⡖⠲⣦⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⠠⠀⠀⠀⠘⣿⡿⠛⠛⢇⠇⣿⢸⣿⣿⣿⣿⣿⣦⠀⠈⡞⠀⠉⠻⣿⣿⠻⣿⠙⠲⣦⡌⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⢀⡀⢦⣀⣿⡇⢠⡍⠈⢀⢻⡀⣿⣿⣿⣿⣿⣿⣧⠰⡇⠸⠲⡄⠈⠿⡄⠙⠛⠳⢦⠙⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠐⡈⣿⣿⣇⢸⣿⡀⠘⢸⡇⣿⣿⣿⣿⣿⣿⣿⠃⡇⠀⠐⣿⠀⠀⠈⠢⠀⠄⠀⠃⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠱⠘⣿⣯⠸⣿⣧⠂⠀⢇⢹⣿⣿⣿⣿⣿⣿⣷⣴⣷⣄⠃⠀⡉⡀⠀⠐⢄⠀⠀⠁⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⢃⢹⣿⠀⢿⣿⠀⡀⢸⡈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠁⡘⣷⣦⣀⠑⣠⣶⣦⡙⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⡄⡟⠀⢸⣿⣆⠻⡌⠣⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡆⢰⣕⣌⡛⠻⠿⣿⣿⣿⣷⡜⣿⣿⣿⣿⣿⣿⣿⣿⣿
// ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠱⠀⢠⢸⣿⣿⣷⡄⠀⠈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡌⢻⣿⣿⣿⣦⡘⢿⡿⢻⣷⢸⣿⣿⣿⣿⣿⣿⣿⣿

UENUM(BlueprintType)
enum EScoreType : uint8
{
    EST_Constant UMETA(DisplayName = "Constant"),
    EST_Evaluated UMETA(DisplayName = "Evaluated")
    // EST_Blackboard
};

UENUM(BlueprintType)
enum EExecutionResult : uint8
{
    Success,
    Aborted,
    Failed
};


class AAIController;
class APawn;
class UService;
class UUtilityManager;
class UConsideration;

UCLASS(MinimalAPI,
       BlueprintType,
       Blueprintable,
       Abstract,
       DefaultToInstanced,
       EditInlineNew,
       DisplayName = "UtilityAction")
class UAction : public UObject
{
    GENERATED_BODY()

private:
    float ActionScore = 0.f;
    bool bCanBeEvaluated = true;

    FTimerHandle _timeoutTimer;

    FRichCurve *ModifierCurve;

private:
    virtual UWorld *GetWorld() const override;
    FORCEINLINE void ResetTimeout();

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

public:
    UPROPERTY(BlueprintReadOnly, Category = "UtilityAI|Action")
    TObjectPtr<AAIController> _controller;
    UPROPERTY(BlueprintReadOnly, Category = "UtilityAI|Action")
    TObjectPtr<APawn> _pawn;
    UPROPERTY(BlueprintReadOnly, Category = "UtilityAI|Action")
    TObjectPtr<UUtilityManager> _manager;

    /** Set this to true if action can be run in parallel with others*/
    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig")
    bool bCanRunConcurent = false;

    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig",
              meta = (EditCondition = "bCanRunConcurent == true", EditConditionHides))
    TSet<TSubclassOf<UAction>> ConcurencyExceptions;

    /** Priority of this action within pool */
    UPROPERTY(EditAnywhere, Category = "ActionConfig", meta = (ClampMin = 1))
    uint32 ActionPriority = 1;

    /** Time while action was closed for evalueating since last execution */
    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig", meta = (ClampMin = .0f))
    float Timeout = .1f;

    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig")
    TEnumAsByte<EScoreType> ScoreType;

    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig",
              meta = (ClampMin = 0.f, ClampMax = 1.f,
                      EditCondition = "ScoreType==EScoreType::EST_Constant",
                      EditConditionHides, EditInline = false))
    float ConstantScore = 0.f;

    UPROPERTY(EditDefaultsOnly, Category = "ActionConfig",
              meta = (EditCondition = "ScoreType==EScoreType::EST_Evaluated",
                      EditConditionHides))
    TArray<FScorer> Scorers;

    // UPROPERTY(EditDefaultsOnly, Category = "ActionConfig")
    // FCurveTableRowHandle UtilityModifier_Curve;

protected:
    UFUNCTION(BlueprintCallable, Category = "UtilityAI|Action", BlueprintPure)
    UUtilityManager *GetUtilityManager() { return _manager; }

    /** Provides action implementation */
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Action")
    void ExecuteAction(AAIController *Controller, APawn *Pawn);

    /** Called when action is finished or aborted */
    UFUNCTION(BlueprintImplementableEvent, Category = "UtilityAI|Action")
    void OnActionFinished(AAIController *Controller, APawn *Pawn, EExecutionResult execResult);

public:
    bool IsFinished = true;

    UAction() {};

    /** Initialize action */
    void Init(TObjectPtr<UUtilityManager> InManager, TMap<FGameplayTag, TObjectPtr<UConsideration>> InConsiderations);
    /** Calculate action score based on score type and linked scorers */
    float EvaluateActionScore();
    /** Call's action implementation */
    void Execute();
    FORCEINLINE float GetActionScore() const { return ActionScore; }
    FORCEINLINE bool CanRunConcurrentlyWith(UAction *OtherAction) const
    {
        return bCanRunConcurent &&
               OtherAction->bCanRunConcurent &&
               OtherAction != this &&
               !ConcurencyExceptions.Contains(OtherAction->GetClass());
    }

    /** @return all scorers related to this action*/
    FORCEINLINE virtual TArray<FScorer> GetScorers()
    {
        return Scorers;
    };

    /** Needs to be called when action is about to finish */
    UFUNCTION(BlueprintCallable, Category = "UtilityAI|Action")
    void FinishExecute(EExecutionResult execResult);

    FORCEINLINE virtual bool CanBeEvaluated() const { return bCanBeEvaluated && IsFinished; };
};