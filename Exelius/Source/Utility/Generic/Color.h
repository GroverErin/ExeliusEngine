#pragma once
#include <cstdint>

/// <summary>
/// This macro should be defined in a configuration file.
/// </summary>
#define EXELIUS_NONPRIMARY_COLORS = 1

/// <summary>
/// Engine namespace. Everything owned by the engine will be inside this namespace.
/// Anything with a "_" prefixed is private to the engine and is not recommended for use by client applications.
/// Example: _Log
/// </summary>
namespace Exelius
{
	/// <summary>
	/// Container for RGBA values. Can be used with both 255/255/255/255 or Hex values.
	/// </summary>
	struct Color
	{
		/// <summary>
		/// Construct with default color of White.
		/// </summary>
		constexpr Color()
			: r(255)
			, g(255)
			, b(255)
			, a(255)
		{
			//
		}

		/// <summary>
		/// Construct with RGB values matching the parameter value. The alpha is MAX and not effected.
		/// </summary>
		/// <param name="color">The value to set the RGB values to.</param>
		constexpr Color(uint8_t color)
			: r(color)
			, g(color)
			, b(color)
			, a(255)
		{
			//
		}

		/// <summary>
		/// Construct with explicitly set RGB values, Alpha is optional and defaults to MAX.
		/// </summary>
		/// <param name="red">Amount of Red in the color.</param>
		/// <param name="green">Amount of Green in the color.</param>
		/// <param name="blue">Amount of blue in the color.</param>
		/// <param name="alpha">Color Transparanency value. Defaults to MAX uint8_t (255).</param>
		constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
			: r(red)
			, g(green)
			, b(blue)
			, a(alpha)
		{
			//
		}

		/// <summary>
		/// Get a color using a single hexadecimal value.
		/// White = 0xFF'FF'FF'FF
		///			   R  G  B  A
		/// </summary>
		/// <param name="hexColor">Hexadecimal value used to create a color.</param>
		/// <returns>A color based on the value passed in.</returns>
		static constexpr Color FromHex(uint32_t hexColor)
		{
			return Color((uint8_t)((hexColor & 0xFF000000) >> 24),
						(uint8_t)((hexColor & 0x00FF0000) >> 16),
						(uint8_t)((hexColor & 0x0000FF00) >> 8),
						(uint8_t)(hexColor & 0x000000FF));
		}

		/// <summary>
		/// Return the color as a single hexidecimal value.
		/// </summary>
		/// <returns>Hexadecimal value used to create a color.</returns>
		constexpr uint32_t GetHex() const
		{
			const uint32_t hexColor = (a) | (b << 8) | (g << 16) | (r << 24);
			return hexColor;
		}

		/// <summary>
		/// Red value.
		/// </summary>
		uint8_t r;

		/// <summary>
		/// Green value.
		/// </summary>
		uint8_t g;

		/// <summary>
		/// Blue value.
		/// </summary>
		uint8_t b;

		/// <summary>
		/// Alpha value.
		/// </summary>
		uint8_t a;
	};

	/// <summary>
	/// Exelius' pre-defined general use colors.
	/// Colors retrieved from http://www.workwithcolor.com/color-chart-full-01.htm
	/// </summary>
	namespace Colors
	{
		static constexpr Color Black(0, 0, 0, 255);
		static constexpr Color Blue(0, 0, 255, 255);
		static constexpr Color Brown(150, 75, 0, 255);
		static constexpr Color Cyan(0, 255, 255, 255);
		static constexpr Color Gray(128, 128, 128, 255);
		static constexpr Color Green(0, 128, 0, 255);
		static constexpr Color Magenta(255, 0, 255, 255);
		static constexpr Color Pink(255, 192, 203, 255);
		static constexpr Color Red(255, 0, 0, 255);
		static constexpr Color White(255, 255, 255, 255);
		static constexpr Color Yellow(255, 255, 0, 255);

#ifdef EXELIUS_NONPRIMARY_COLORS
		static constexpr Color AirForceBlue(93, 138, 168, 255);
		static constexpr Color AliceBlue(240, 248, 255, 255);
		static constexpr Color Alizarin(227, 38, 54, 255);
		static constexpr Color Amaranth(229, 43, 80, 255);
		static constexpr Color Amber(255, 191, 0, 255);
		static constexpr Color AndroidGreen(164, 198, 57, 255);
		static constexpr Color AppleGreen(141, 182, 0, 255);
		static constexpr Color Apricot(251, 206, 177, 255);
		static constexpr Color Aquamarine(127, 255, 212, 255);
		static constexpr Color ArmyGreen(75, 83, 32, 255);
		static constexpr Color Arsenic(59, 68, 75, 255);
		static constexpr Color ArylideYellow(233, 214, 107, 255);
		static constexpr Color AshGrey(178, 190, 181, 255);
		static constexpr Color Asparagus(135, 169, 107, 255);
		static constexpr Color AtomicTangerine(255, 153, 102, 255);
		static constexpr Color Auburn(109, 53, 26, 255);
		static constexpr Color Azure(0, 127, 255, 255);
		static constexpr Color BabyBlue(137, 207, 240, 255);
		static constexpr Color BabyBlueEyes(161, 202, 241, 255);
		static constexpr Color BabyPink(244, 194, 194, 255);
		static constexpr Color BananaYellow(255, 209, 42, 255);
		static constexpr Color BattleshipGrey(132, 132, 130, 255);
		static constexpr Color Bazaar(152, 119, 123, 255);
		static constexpr Color Beige(245, 245, 220, 255);
		static constexpr Color Bistre(61, 43, 31, 255);
		static constexpr Color BleuDeFrance(49, 140, 231, 255);
		static constexpr Color Blond(250, 240, 190, 255);
		static constexpr Color Blush(222, 93, 131, 255);
		static constexpr Color Bole(121, 68, 59, 255);
		static constexpr Color BostonUniversityRed(204, 0, 0, 255);
		static constexpr Color Brass(181, 166, 66, 255);
		static constexpr Color BrightGreen(102, 255, 0, 255);
		static constexpr Color BrightLavender(191, 148, 228, 255);
		static constexpr Color BrightMaroon(195, 33, 72, 255);
		static constexpr Color BrightPink(255, 0, 127, 255);
		static constexpr Color BrightTurquoise(8, 232, 222, 255);
		static constexpr Color BrightUbe(209, 159, 232, 255);
		static constexpr Color BritishRacingGreen(0, 66, 37, 255);
		static constexpr Color Bronze(205, 127, 50, 255);
		static constexpr Color BubbleGum(255, 193, 204, 255);
		static constexpr Color Bubbles(231, 254, 255, 255);
		static constexpr Color Buff(240, 220, 130, 255);
		static constexpr Color Burgundy(128, 0, 32, 255);
		static constexpr Color Burlywood(222, 184, 135, 255);
		static constexpr Color BurntOrange(204, 85, 0, 255);
		static constexpr Color BurntSienna(233, 116, 81, 255);
		static constexpr Color BurntUmber(138, 51, 36, 255);
		static constexpr Color Byzantine(189, 51, 164, 255);
		static constexpr Color Byzantium(112, 41, 99, 255);
		static constexpr Color Cadet(83, 104, 120, 255);
		static constexpr Color CadmiumGreen(0, 107, 60, 255);
		static constexpr Color CadmiumOrange(237, 135, 45, 255);
		static constexpr Color CadmiumRed(227, 0, 34, 255);
		static constexpr Color CambridgeBlue(163, 193, 173, 255);
		static constexpr Color CamouflageGreen(120, 134, 107, 255);
		static constexpr Color CanaryYellow(255, 239, 0, 255);
		static constexpr Color CandyAppleRed(255, 8, 0, 255);
		static constexpr Color Cardinal(196, 30, 58, 255);
		static constexpr Color CaribbeanGreen(0, 204, 153, 255);
		static constexpr Color Carmine(150, 0, 24, 255);
		static constexpr Color CarolinaBlue(153, 186, 221, 255);
		static constexpr Color CarrotOrange(237, 145, 33, 255);
		static constexpr Color Ceil(146, 161, 207, 255);
		static constexpr Color Celadon(172, 225, 175, 255);
		static constexpr Color Cerulean(0, 123, 167, 255);
		static constexpr Color CeruleanBlue(42, 82, 190, 255);
		static constexpr Color Chamoisee(160, 120, 90, 255);
		static constexpr Color Champagne(247, 231, 206, 255);
		static constexpr Color Charcoal(54, 69, 79, 255);
		static constexpr Color Chartreuse(223, 255, 0, 255);
		static constexpr Color Cherry(222, 49, 99, 255);
		static constexpr Color CherryBlossomPink(255, 183, 197, 255);
		static constexpr Color Chestnut(205, 92, 92, 255);
		static constexpr Color Chocolate(123, 63, 0, 255);
		static constexpr Color ChromeYellow(255, 167, 0, 255);
		static constexpr Color Cinereous(152, 129, 123, 255);
		static constexpr Color Cinnabar(227, 66, 52, 255);
		static constexpr Color Cinnamon(210, 105, 30, 255);
		static constexpr Color Citrine(228, 208, 10, 255);
		static constexpr Color ClassicRose(251, 204, 231, 255);
		static constexpr Color Clover(0, 255, 111, 255);
		static constexpr Color Cobalt(0, 71, 171, 255);
		static constexpr Color ColumbiaBlue(155, 221, 255, 255);
		static constexpr Color CoolBlack(0, 46, 99, 255);
		static constexpr Color CoolGrey(140, 146, 172, 255);
		static constexpr Color Copper(184, 115, 51, 255);
		static constexpr Color Coquelicot(255, 56, 0, 255);
		static constexpr Color Coral(255, 127, 80, 255);
		static constexpr Color Cordovan(137, 63, 69, 255);
		static constexpr Color Corn(251, 236, 93, 255);
		static constexpr Color CornellRed(179, 27, 27, 255);
		static constexpr Color CornflowerBlue(100, 149, 237, 255);
		static constexpr Color Cornsilk(255, 248, 220, 255);
		static constexpr Color Cream(255, 253, 208, 255);
		static constexpr Color Crimson(220, 20, 60, 255);
		static constexpr Color Daffodil(255, 255, 49, 255);
		static constexpr Color Dandelion(240, 225, 48, 255);
		static constexpr Color DarkBlue(0, 0, 139, 255);
		static constexpr Color DarkBrown(101, 67, 33, 255);
		static constexpr Color DarkByzantium(93, 57, 84, 255);
		static constexpr Color DarkCandyAppleRed(164, 0, 0, 255);
		static constexpr Color DarkCerulean(8, 69, 126, 255);
		static constexpr Color DarkChestnut(152, 105, 96, 255);
		static constexpr Color DarkCoral(205, 91, 69, 255);
		static constexpr Color DarkCyan(0, 139, 139, 255);
		static constexpr Color DarkGoldenrod(184, 134, 11, 255);
		static constexpr Color DarkGreen(1, 50, 32, 255);
		static constexpr Color DarkJungleGreen(26, 36, 33, 255);
		static constexpr Color DarkKhaki(189, 183, 107, 255);
		static constexpr Color DarkLava(72, 60, 50, 255);
		static constexpr Color DarkLavender(115, 79, 150, 255);
		static constexpr Color DarkMagenta(139, 0, 139, 255);
		static constexpr Color DarkMidnightBlue(0, 51, 102, 255);
		static constexpr Color DarkOliveGreen(85, 107, 47, 255);
		static constexpr Color DarkOrange(255, 140, 0, 255);
		static constexpr Color DarkPastelBlue(119, 158, 203, 255);
		static constexpr Color DarkPastelGreen(3, 192, 60, 255);
		static constexpr Color DarkPastelPurple(150, 111, 214, 255);
		static constexpr Color DarkPastelRed(194, 59, 34, 255);
		static constexpr Color DarkPink(231, 84, 128, 255);
		static constexpr Color DarkPowderBlue(0, 51, 153, 255);
		static constexpr Color DarkRaspberry(135, 38, 87, 255);
		static constexpr Color DarkSalmon(233, 150, 122, 255);
		static constexpr Color DarkScarlet(86, 3, 25, 255);
		static constexpr Color DarkSienna(60, 20, 20, 255);
		static constexpr Color DarkSlateGray(47, 79, 79, 255);
		static constexpr Color DarkSpringGreen(23, 114, 69, 255);
		static constexpr Color DarkTan(145, 129, 81, 255);
		static constexpr Color DarkTangerine(255, 168, 18, 255);
		static constexpr Color DarkTerraCotta(204, 78, 92, 255);
		static constexpr Color DarkViolet(148, 0, 211, 255);
		static constexpr Color DavySGrey(85, 85, 85, 255);
		static constexpr Color Denim(21, 96, 189, 255);
		static constexpr Color Desert(193, 154, 107, 255);
		static constexpr Color DesertSand(237, 201, 175, 255);
		static constexpr Color DimGray(105, 105, 105, 255);
		static constexpr Color DollarBill(133, 187, 101, 255);
		static constexpr Color DukeBlue(0, 0, 156, 255);
		static constexpr Color EarthYellow(225, 169, 95, 255);
		static constexpr Color Eggplant(97, 64, 81, 255);
		static constexpr Color Emerald(80, 200, 120, 255);
		static constexpr Color Fawn(229, 170, 112, 255);
		static constexpr Color FerrariRed(255, 28, 0, 255);
		static constexpr Color FireEngineRed(206, 22, 32, 255);
		static constexpr Color Firebrick(178, 34, 34, 255);
		static constexpr Color Flame(226, 88, 34, 255);
		static constexpr Color FlamingoPink(252, 142, 172, 255);
		static constexpr Color Flavescent(247, 233, 142, 255);
		static constexpr Color ForestGreen(1, 68, 33, 255);
		static constexpr Color Gamboge(228, 155, 15, 255);
		static constexpr Color GhostWhite(248, 248, 255, 255);
		static constexpr Color Glaucous(96, 130, 182, 255);
		static constexpr Color GoldenBrown(153, 101, 21, 255);
		static constexpr Color GoldenYellow(255, 223, 0, 255);
		static constexpr Color Goldenrod(218, 165, 32, 255);
		static constexpr Color Iceberg(113, 166, 210, 255);
		static constexpr Color Icterine(252, 247, 94, 255);
		static constexpr Color Inchworm(178, 236, 93, 255);
		static constexpr Color IndiaGreen(19, 136, 8, 255);
		static constexpr Color IndianRed(255, 92, 92, 255);
		static constexpr Color IndianYellow(227, 168, 87, 255);
		static constexpr Color InternationalKleinBlue(0, 47, 167, 255);
		static constexpr Color Ivory(255, 255, 240, 255);
		static constexpr Color Jade(0, 168, 107, 255);
		static constexpr Color Jasper(215, 59, 62, 255);
		static constexpr Color Khaki(195, 176, 145, 255);
		static constexpr Color Lavender(181, 126, 220, 255);
		static constexpr Color LavenderBlue(204, 204, 255, 255);
		static constexpr Color LavenderBlush(255, 240, 245, 255);
		static constexpr Color LavenderGray(196, 195, 208, 255);
		static constexpr Color LawnGreen(124, 252, 0, 255);
		static constexpr Color Lemon(255, 247, 0, 255);
		static constexpr Color Lime(191, 255, 0, 255);
		static constexpr Color Mahogany(192, 64, 0, 255);
		static constexpr Color Maroon(128, 0, 0, 255);
		static constexpr Color MidnightBlue(25, 25, 112, 255);
		static constexpr Color Mint(62, 180, 137, 255);
		static constexpr Color Mustard(255, 219, 88, 255);
		static constexpr Color NavyBlue(0, 0, 128, 255);
		static constexpr Color Ochre(204, 119, 34, 255);
		static constexpr Color Olive(128, 128, 0, 255);
		static constexpr Color Orange(255, 127, 0, 255);
		static constexpr Color OxfordBlue(0, 33, 71, 255);
		static constexpr Color PastelBlue(174, 198, 207, 255);
		static constexpr Color PastelBrown(131, 105, 83, 255);
		static constexpr Color PastelGray(207, 207, 196, 255);
		static constexpr Color PastelGreen(119, 221, 119, 255);
		static constexpr Color PastelMagenta(244, 154, 194, 255);
		static constexpr Color PastelOrange(255, 179, 71, 255);
		static constexpr Color PastelPink(255, 209, 220, 255);
		static constexpr Color PastelPurple(179, 158, 181, 255);
		static constexpr Color PastelRed(255, 105, 97, 255);
		static constexpr Color PastelViolet(203, 153, 201, 255);
		static constexpr Color PastelYellow(253, 253, 150, 255);
		static constexpr Color Peach(255, 229, 180, 255);
		static constexpr Color Pear(209, 226, 49, 255);
		static constexpr Color Pearl(240, 234, 214, 255);
		static constexpr Color Peridot(230, 226, 0, 255);
		static constexpr Color PineGreen(1, 121, 111, 255);
		static constexpr Color Pistachio(147, 197, 114, 255);
		static constexpr Color Platinum(229, 228, 226, 255);
		static constexpr Color Plum(142, 69, 133, 255);
		static constexpr Color PortlandOrange(255, 90, 54, 255);
		static constexpr Color Prune(112, 28, 28, 255);
		static constexpr Color Pumpkin(255, 117, 24, 255);
		static constexpr Color PurpleHeart(105, 53, 156, 255);
		static constexpr Color Raspberry(227, 11, 93, 255);
		static constexpr Color RawUmber(130, 102, 68, 255);
		static constexpr Color RifleGreen(65, 72, 51, 255);
		static constexpr Color Rosewood(101, 0, 11, 255);
		static constexpr Color RoyalBlue(0, 35, 102, 255);
		static constexpr Color Ruby(224, 17, 95, 255);
		static constexpr Color Rust(183, 65, 14, 255);
		static constexpr Color SafetyOrange(255, 103, 0, 255);
		static constexpr Color Saffron(244, 196, 48, 255);
		static constexpr Color Salmon(255, 140, 105, 255);
		static constexpr Color Sand(194, 178, 128, 255);
		static constexpr Color SandDune(150, 113, 23, 255);
		static constexpr Color Sandstorm(236, 213, 64, 255);
		static constexpr Color Sapphire(8, 37, 103, 255);
		static constexpr Color SealBrown(50, 20, 20, 255);
		static constexpr Color Seashell(255, 245, 238, 255);
		static constexpr Color Sepia(112, 66, 20, 255);
		static constexpr Color Shadow(138, 121, 93, 255);
		static constexpr Color Silver(192, 192, 192, 255);
		static constexpr Color Sinopia(203, 65, 11, 255);
		static constexpr Color SkyBlue(135, 206, 235, 255);
		static constexpr Color SkyMagenta(207, 113, 175, 255);
		static constexpr Color Snow(255, 250, 250, 255);
		static constexpr Color SpringBud(167, 252, 0, 255);
		static constexpr Color SteelBlue(70, 130, 180, 255);
		static constexpr Color Straw(228, 217, 111, 255);
		static constexpr Color Sunset(250, 214, 165, 255);
		static constexpr Color Tangerine(242, 133, 0, 255);
		static constexpr Color Teal(0, 128, 128, 255);
		static constexpr Color TerraCotta(226, 114, 91, 255);
		static constexpr Color TitaniumYellow(238, 230, 0, 255);
		static constexpr Color TropicalRainForest(0, 117, 94, 255);
		static constexpr Color Turquoise(48, 213, 200, 255);
		static constexpr Color Ultramarine(18, 10, 143, 255);
		static constexpr Color UnitedNationsBlue(91, 146, 229, 255);
		static constexpr Color Vanilla(243, 229, 171, 255);
		static constexpr Color Violet(143, 0, 255, 255);
		static constexpr Color Wheat(245, 222, 179, 255);
		static constexpr Color WhiteSmoke(245, 245, 245, 255);
		static constexpr Color Xanadu(115, 134, 120, 255);
		static constexpr Color YaleBlue(15, 77, 146, 255);
#endif // EXELIUS_NONPRIMARY_COLORS
	}
}