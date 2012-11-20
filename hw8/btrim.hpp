#ifndef BTRIM_HPP
#define BTRIM_HPP
 
#include <string>
#include <locale>
 
/*
 * Strip leading whitespace from a string.
 *
 * @param[in] str Reference to the string to be trimmed. Modified in-place.
 * @param[in] loc Locale to use (optional).
 * @return        void
 */
void ltrim(std::string& str, const std::locale& loc = std::locale());

/*
 * Strip trailing whitespace from a string.
 *
 * @param[in] str Reference to the string to be trimmed. Modified in-place.
 * @param[in] loc Locale to use (optional).
 * @return        void
 */
void rtrim(std::string& str, const std::locale& loc = std::locale());

/*
 * Strip both leading and trailing whitespace from a string.
 *
 * @param[in] str Reference to the string to be trimmed. Modified in-place.
 * @param[in] loc Locale to use (optional).
 * @return        void
 */
void btrim(std::string& str, const std::locale& loc = std::locale());

#endif // BTRIM_HPP
