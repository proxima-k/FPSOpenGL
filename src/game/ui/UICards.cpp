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
    {sineCardTexture, "sin(x)  -> The card moves up and down, deals more damage"},
    {cosineCardTexture, "cos(x)  -> The card moves left to right, deals more damage"},
    {placeholder1card, "3x  -> Launches three cards"},
    {placeholder2card, "-(x-1)^2 + 1  -> Has a boomerang effect, deals more damage"},
    {placeholder3card, "X^2 + Y^2 = R  -> A card that circles around you"},
    {passivedamagecard, "dmg(x)  -> Boosts your damage"},
    {passivespeedcard, "spd(x)  -> Makes you move faster"},
    {passivedashcard, "dash(x)  -> Increase dash length"}
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

        // set description based on hovered
        // if is hovered, get the description from the map and set it to description text
        if (isHovered)
            descriptionTxt = cardDescriptions[selectedTextures[i]];


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

void UICards::deckShowcase(ImVec2 deckPos, std::queue<Card*> queue, ImVec2 cardPosCenter, ImVec2 cardSize, bool showCooldown, float cooldownLeftPercentage)
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
        void* targetCardTexture = nullptr;
        switch (tempVector[i]->getCardType())
        {
        case Card::CardType::Sine:
            targetCardTexture = (void*)(intptr_t)sineCardTexture;
            break;

        case Card::CardType::Cosine:
            targetCardTexture = (void*)(intptr_t)cosineCardTexture;
            break;

        case Card::CardType::Placeholder1:
            targetCardTexture = (void*)(intptr_t)placeholder1card;
            break;

        case Card::CardType::Placeholder2:
            targetCardTexture = (void*)(intptr_t)placeholder2card;
            break;

        case Card::CardType::Placeholder3:
            targetCardTexture = (void*)(intptr_t)placeholder3card;
            break;

        case Card::CardType::PassiveDamage:
            targetCardTexture = (void*)(intptr_t)passivedamagecard;
            break;

        case Card::CardType::PassiveSpeed:
            targetCardTexture = (void*)(intptr_t)passivespeedcard;
            break;

        case Card::CardType::PassiveDash:
            targetCardTexture = (void*)(intptr_t)passivedashcard;
            break;

        default:
            targetCardTexture = (void*)(intptr_t)basicCardTexture;
            break;
        }

        float deckXOffset((2 - (2 - 1) / 2.0f) * selectionXSpacing);
        float deckYOffset((i - (2 - 1) / 2.0f) * deckYSpacing - (queue.size() * deckYSpacing / 2));
        ImVec2 cardPos(deckPos.x, deckPos.y + deckYOffset);

        // if highlight card
        if (showCooldown && i == 0 && cooldownLeftPercentage > 0.f) {
            /*ImGui::SetCursorPos(cardPos);
            ImGui::Image(targetCardTexture, cardSize);*/

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddImage(
                targetCardTexture,
                ImVec2(cardPos.x, cardPos.y),
                ImVec2(cardPos.x + cardSize.x, cardPos.y + cardSize.y * cooldownLeftPercentage),
                ImVec2(0.0f, 0.0f),
                ImVec2(1.0f, cooldownLeftPercentage),
                ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 1.f))
            );

            drawList->AddImage(
                targetCardTexture,
                ImVec2(cardPos.x, cardPos.y + cardSize.y * cooldownLeftPercentage),
                ImVec2(cardPos.x + cardSize.x, cardPos.y + cardSize.y),
                ImVec2(0.0f, cooldownLeftPercentage),
                ImVec2(1.0f, 1.0f),
                ImGui::GetColorU32(ImVec4(0.5f, 0.5f, 0.5f, 1.f))
            );
        }
        else {
            ImGui::SetCursorPos(cardPos);
            ImGui::Image(targetCardTexture, cardSize);
        }
        // then perform division
        // else just create default image with cursor position
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
