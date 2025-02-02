//   _______ _______ ______ _______ ___ ___ _______ _______ _______ 
//  |       |   |   |   __ \       |   |   |    ___|       |    |  |
//  |   -   |   |   |      <   -   |   |   |    ___|   -   |       |
//  |_______|_______|___|__|_______|\_____/|_______|_______|__|____|
//  \\ harry denholm \\ ishani            ishani.org/shelf/ouroveon/
//

#include "base/utils.h"
#include "endlesss/core.types.h"
#include "endlesss/toolkit.population.h"

namespace ImGui {
namespace ux {

struct UserSelector
{
    static constexpr float cDefaultWidthForUserSize = 180.0f;   // should comfortably fit the 16-letter username limit

    UserSelector() = default;
    UserSelector( const std::string_view defaultUsername )
        : m_username( defaultUsername )
    {}

    bool imgui( const char* widgetID, const endlesss::toolkit::PopulationQuery& population, float itemWidth = -1.0f );

    ouro_nodiscard const std::string& getUsername() const { return m_username; }
    void setUsername( const std::string_view username ) { m_username = username; }

    ouro_nodiscard bool isEmpty() const { return m_username.empty(); }

private:

    using PopulationAutocomplete = endlesss::toolkit::PopulationQuery::Result;

    int32_t                     m_suggestionIndex = -1;
    std::string                 m_username;
    PopulationAutocomplete      m_autocompletion;
};

} // namespace ux
} // namespace ImGui
