#include "../ui/UICards.h"

#include <map>

void UICards::renderCardSelection(ImVec2 windowSize) {
    ImVec2 descriptionBoxSize = { windowSize.x / 1.5f, 120 };

    ImGui::SetCursorPos({ (windowSize.x / 2 - descriptionBoxSize.x / 2), (windowSize.y * 0.7f - descriptionBoxSize.y / 2) });
    ImGui::Image((void*)(intptr_t)game->textureManager->getTexture(""), descriptionBoxSize);

    ImVec2 textSize = ImGui::CalcTextSize(descriptionTxt.c_str());
    float textX = (windowSize.x / 2) - (textSize.x / 2);
    float textY = (windowSize.y * 0.7f) - descriptionBoxSize.y / 4;

    ImGui::SetCursorPos({ textX, textY });
    ImGui::Text(descriptionTxt.c_str());

    std::map<GLuint, std::string> cardDescriptions = {
    {sineCardTexture, "sin(x) — The card moves left to right, deals more damage"},
    {cosineCardTexture, "cos(x) — The card moves up and down, deals more damage"},
    {placeholder1card, "3x - Launches three cards"},
    {placeholder2card, "x^2 - Has a boomerang effect, deals more damage"},
    {placeholder3card, "d * pi - A card that circles around you"},
    {passivedamagecard, "dmg(x) - Boosts your damage"},
    {passivespeedcard, "spd(x) - Makes you move faster"},
    {passivedashcard, "dash(x) Increeses dash lenght"}
    };

    int cardsPerRow = (selectionAmount > 2) ? (selectionAmount + 1) / 2 : selectionAmount;
    int rows = (selectionAmount + cardsPerRow - 1) / cardsPerRow;

    float halfXOffset = (cardsPerRow - 1) / 2.0f;
    float halfYOffset = (rows - 1) / 2.0f;

    ImVec2 cardSize(150, 220);
    ImVec2 cardPosCenter((windowSize.x) / 2.0f - (cardSize.x / 2), (windowSize.y) / 2.0f - (cardSize.y / 2));

    randomizeCards();

    for (int i = 0; i < selectionAmount; i++) {
        int row = i / cardsPerRow;
        int indexInRow = i % cardsPerRow;

        ImVec2 offset = ImVec2((indexInRow - halfXOffset) * selectionXSpacing, (row - halfYOffset) * selectionYSpacing);
        ImVec2 cardPos(cardPosCenter.x + offset.x, cardPosCenter.y + offset.y);

        ImGui::SetCursorPos(cardPos);
        ImGui::PushID(i);

        bool isHovered = ImGui::IsMouseHoveringRect(cardPos, ImVec2(cardPos.x + cardSize.x, cardPos.y + cardSize.y));

        float scaleFactor = isHovered ? 1.1f : 1.0f;

        ImVec2 scaledCardSize = ImVec2(cardSize.x * scaleFactor, cardSize.y * scaleFactor);
        ImVec2 scaleOffset = ImVec2((scaledCardSize.x - cardSize.x) / 2, (scaledCardSize.y - cardSize.y) / 2);
        ImVec2 adjustedPos = ImVec2(cardPos.x - scaleOffset.x, cardPos.y - scaleOffset.y);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 0.f));          // Button idle color
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.f, 1.f, 1.f, 1.f));   // Button hover color
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.f, 1.f, 1.f, 0.f));    // Button active color

        ImGui::SetCursorPos(adjustedPos);
        bool clicked = ImGui::ImageButton((void*)(intptr_t)selectedTextures[i], scaledCardSize);

        if (clicked)
        {
            std::cout << "Pressed button " << i << std::endl;

            if (selectedTextures[i] == sineCardTexture) {
                shooter->cardQueue.push(new SineCard(glm::vec3(0)));
            }
            else if (selectedTextures[i] == cosineCardTexture) {
                shooter->cardQueue.push(new CosineCard(glm::vec3(0)));
            }
            else if (selectedTextures[i] == placeholder1card) {
                shooter->cardQueue.push(new PlaceHolderCard1(glm::vec3(0)));
            }
            else if (selectedTextures[i] == placeholder2card) {
                shooter->cardQueue.push(new PlaceHolderCard2(glm::vec3(0)));
            }
            else if (selectedTextures[i] == placeholder3card) {
                shooter->cardQueue.push(new PlaceHolderCard3(glm::vec3(0)));
            }
            else if (selectedTextures[i] == passivedamagecard) {
                auto damagePassive = new DamagePassive(glm::vec3(0));
                shooter->cardPassivesQueue.push(damagePassive);
                damagePassive->applyPassiveEffect();
            }
            else if (selectedTextures[i] == passivespeedcard) {
                auto passivespeedcard = new SpeedPassive(glm::vec3(0));
                shooter->cardPassivesQueue.push(passivespeedcard);
                passivespeedcard->applyPassiveEffect();
            }
            else if (selectedTextures[i] == passivedashcard) {
                auto dashbuffcard = new DashPassive(glm::vec3(0));
                shooter->cardPassivesQueue.push(dashbuffcard);
                dashbuffcard->applyPassiveEffect();
            }

            game->changeState(GameStateManager::State::Playing);

            if (shooter->cardQueue.size() > maxCardHandSize) {
                shooter->cardQueue.pop();
            }
        }
        ImGui::PopStyleColor(3);


        ImGui::PopID();
    }
}



void UICards::deckShowcase(ImVec2 deckPos, std::queue<Card*> queue, ImVec2 cardPosCenter, ImVec2 cardSize, bool highlightCard)
{
    float deckYSpacing = 20;

    float deckXOffset((2 - (2 - 1) / 2.0f) * selectionXSpacing);
    float deckYOffset((1 - (2 - 1) / 2.0f) * deckYSpacing - (shooter->cardQueue.size() * deckYSpacing / 2));

    ImVec2 cardPos(deckPos.x, deckPos.y + deckYOffset);

    std::vector<Card*> tempVector;
    std::queue<Card*> tempQueue = queue;

    if (queue.size() <= 0)
    {
        ImGui::SetCursorPos(cardPos);
        ImGui::Image((void*)(intptr_t)emptydeck, cardSize);

        return;
    }

    while (!tempQueue.empty()) {
        tempVector.push_back(tempQueue.front());
        tempQueue.pop();
    }

    for (int i = queue.size() - 1; i >= 0; i--)
    {
        float deckXOffset((2 - (2 - 1) / 2.0f) * selectionXSpacing);
        float deckYOffset((i - (2 - 1) / 2.0f) * deckYSpacing - (queue.size() * deckYSpacing / 2));

        if (highlightCard && i == 0) {
            highlightProgress += ImGui::GetIO().DeltaTime * 5;

            if (highlightProgress > 1.0f) highlightProgress = 1.0f;

            highlightCurrentOffset = glm::mix(0.0f, highlightOffsetMax, highlightProgress);
            deckYOffset -= highlightCurrentOffset;
        }
        else if (!highlightCard) {
            highlightProgress = 0.0f;
        }

        ImVec2 cardPos(deckPos.x, deckPos.y + deckYOffset);

        ImGui::SetCursorPos(cardPos);

        switch (tempVector[i]->getCardType())
        {
        case Card::CardType::Sine:
            ImGui::Image((void*)(intptr_t)sineCardTexture, cardSize);
            break;

        case Card::CardType::Cosine:
            ImGui::Image((void*)(intptr_t)cosineCardTexture, cardSize);
            break;

        case Card::CardType::Placeholder1:
            ImGui::Image((void*)(intptr_t)placeholder1card, cardSize);
            break;

        case Card::CardType::Placeholder2:
            ImGui::Image((void*)(intptr_t)placeholder2card, cardSize);
            break;

        case Card::CardType::Placeholder3:
            ImGui::Image((void*)(intptr_t)placeholder3card, cardSize);
            break;

        case Card::CardType::PassiveDamage:
            ImGui::Image((void*)(intptr_t)passivedamagecard, cardSize);
        break;

        case Card::CardType::PassiveSpeed:
            ImGui::Image((void*)(intptr_t)passivespeedcard, cardSize);
        break;

        case Card::CardType::PassiveDash:
            ImGui::Image((void*)(intptr_t)passivedashcard, cardSize);
        break;

        default:
            ImGui::Image((void*)(intptr_t)basicCardTexture, cardSize);
            break;
        }
    }
}
void UICards::randomizeCards()
{
    std::vector<GLuint> cardTextures;
    cardTextures.insert(cardTextures.end(), { passivespeedcard, passivedashcard, passivedamagecard });

    if(shooter->cardQueue.size() < 3)
    cardTextures.insert(cardTextures.end(), { sineCardTexture, cosineCardTexture, placeholder1card, placeholder2card, placeholder3card });

    if (!cardsRandomized) {
        std::random_device rd;
        std::mt19937 rng(rd());
        selectedTextures.resize(selectionAmount);

        for (int i = 0; i < selectionAmount; i++) 
        {
            std::uniform_int_distribution<int> dist(0, cardTextures.size() - 1);
            selectedTextures[i] = cardTextures[dist(rng)];
        }

        cardsRandomized = true;
    }
}
