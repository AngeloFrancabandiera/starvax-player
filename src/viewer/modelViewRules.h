#include "Qt"

/**
 * custom rules to be used in model view framework to retrieve
 * data required by models
 */
namespace modelViewRules
{

/**
 * @brief Rule used by custom viewers. Returned data are custum structures.
 */
const int Binary = Qt::UserRole + 1;

/**
 * @brief Rule used by show manager to save text data into show file. QString is returned
 */
const int Serialize = Qt::UserRole + 2;

/**
 * @brief this string is saved in the show file. It should identify an event,
 *    given it's type.
 */
const int StringId = Qt::UserRole + 3;

/**
 * @brief used in 'setData' only to create a new item. Variant data is a string
 *   that defines how to create the new item.
 */
const int CreateFromString = Qt::UserRole + 4;
}
