#include "../ui/UICards.h"

#include <map>

void UICards::renderCardSelection(ImVec2 windowSize)
{
    ImVec2 descriptionBoxSize = { windowSize.x / 1.5f, 120 };

    ImGui::SetCursorPos({ (windowSize.x / 2 - descriptionBoxSize.x / 2), (windowSize.y * 0.7f - descriptionBoxSize.y / 2) });
    ImGui::Image((void*)(intptr_t)game->textureManager->getTexture(""), descriptionBoxSize);

    ImVec2 textSize = ImGui::CalcTextSize(descriptionTxt.c_str());
    float textX = (windowSize.x / 2) - (textSize.x / 2);
    float textY = (windowSize.y * 0.7f) - descriptionBoxSize.y / 4;

    ImGui::SetCursorPos({ textX, textY });
    ImGui::Text(descriptionTxt.c_str());

    std::map<GLuint, std::string> cardDescriptions = {
    {sineCardTexture, "sin(x) — Applies a sine wave effect with amplitude modulation."},
    {cosineCardTexture, "cos(x) — Applies a cosine wave effect with frequency adjustment."},
    {placeholder1card, "f(x) = x^2 — Placeholder effect representing a quadratic increase."},
    {placeholder2card, "f(x) = log(x) — Placeholder effect representing logarithmic scaling."},
    {placeholder3card, "f(x) = e^x — Placeholder effect representing exponential growth."},
    {passivedamagecard, "dmg(x) = base_damage * (1 + 0.1x) — Increases damage by 10% per level."},
    {passivespeedcard, "spd(x) = base_speed * (1 + 0.05x) — Increases speed by 5% per level."},
    {passivedashcard, "dash(x) = base_dash_distance * (1 + 0.2x) — Increases dash distance by 20% per level."}
    };


    int cardsPerRow = (selectionAmount > 2) ? (selectionAmount + 1) / 2 : selectionAmount;
    int rows = (selectionAmount + cardsPerRow - 1) / cardsPerRow;

    float halfXOffset = (cardsPerRow - 1) / 2.0f;
    float halfYOffset = (rows - 1) / 2.0f;

    ImVec2 cardSize(150, 220);
    ImVec2 cardPosCenter((windowSize.x) / 2.0f - (cardSize.x / 2), (windowSize.y) / 2.0f - (cardSize.y / 2));

    randomizeCards();

    // card states
    std::vector<float> cardScales(selectionAmount, 1.0f);
    std::vector<float> cardTimes(selectionAmount, 0.0f);
    std::vector<bool> cardIsAnimating(selectionAmount, false);

    for (int i = 0; i < selectionAmount; i++)
    {
        int row = i / cardsPerRow;
        int indexInRow = i % cardsPerRow;

        ImVec2 offset = ImVec2((indexInRow - halfXOffset) * selectionXSpacing, (row - halfYOffset) * selectionYSpacing);
        ImVec2 cardPos(cardPosCenter.x + offset.x, cardPosCenter.y + offset.y);

        ImGui::SetCursorPos(cardPos);
        ImGui::PushID(i);

        bool isHovered = ImGui::IsMouseHoveringRect(cardPos, ImVec2(cardPos.x + cardSize.x, cardPos.y + cardSize.y));
        float targetScale = isHovered ? scaleMultiplier : 1.0f;

        if (isHovered) {
            descriptionTxt = cardDescriptions[selectedTextures[i]];

            if (!cardIsAnimating[i])
                cardIsAnimating[i] = true;
        }

        if (cardIsAnimating[i]) {

            cardTimes[i] += ImGui::GetIO().DeltaTime * scaleSpeed;
            if (cardTimes[i] > 1.0f) cardTimes[i] = 1.0f;

            cardScales[i] = 1.0f + (targetScale - 1.0f) * xyzmath::EaseOutElastic(cardTimes[i]);

            if (!isHovered && cardTimes[i] >= 1.0f) {
                cardIsAnimating[i] = false;
                cardTimes[i] = 0.0f;
            }
        }

        ImVec2 scaledCardSize = ImVec2(cardSize.x * cardScales[i], cardSize.y * cardScales[i]);
        ImVec2 scaleOffset = ImVec2((scaledCardSize.x - cardSize.x) / 2, (scaledCardSize.y - cardSize.y) / 2);
        ImVec2 adjustedPos = ImVec2(cardPos.x - scaleOffset.x, cardPos.y - scaleOffset.y);

        ImGui::SetCursorPos(adjustedPos);
        bool clicked = ImGui::ImageButton((void*)(intptr_t)selectedTextures[i], scaledCardSize);

        if (clicked)
        {
            std::cout << "Pressed button " << i << std::endl;

            if (selectedTextures[i] == sineCardTexture)
            {
                shooter->cardQueue.push(new SineCard(glm::vec3(0)));
            }
            else if (selectedTextures[i] == cosineCardTexture)
            {
                shooter->cardQueue.push(new CosineCard(glm::vec3(0)));
            }
            else if (selectedTextures[i] == placeholder1card)
            {
                shooter->cardQueue.push(new PlaceHolderCard1(glm::vec3(0)));
            }
            else if (selectedTextures[i] == placeholder2card)
            {
                shooter->cardQueue.push(new PlaceHolderCard2(glm::vec3(0)));
            }
            else if (selectedTextures[i] == placeholder3card)
            {
                shooter->cardQueue.push(new PlaceHolderCard3(glm::vec3(0)));
            }
            else if (selectedTextures[i] == passivedamagecard)
            {
                auto damagePassive = new DamagePassive(glm::vec3(0));

				shooter->cardPassivesQueue.push(damagePassive);
                damagePassive->applyPassiveEffect();
			}
            else if (selectedTextures[i] == passivespeedcard)
            {
                auto passivespeedcard = new SpeedPassive(glm::vec3(0));

                shooter->cardPassivesQueue.push(passivespeedcard);
                passivespeedcard->applyPassiveEffect();
            }
            else if (selectedTextures[i] == passivedashcard)
            {
                auto dashbuffcard = new DashPassive(glm::vec3(0));

                shooter->cardPassivesQueue.push(dashbuffcard);
                dashbuffcard->applyPassiveEffect();
            }

            game->changeState(GameStateManager::State::Playing);

            if (shooter->cardQueue.size() > maxCardHandSize)
            {
                shooter->cardQueue.pop();
            }
        }

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
    std::vector<GLuint> cardTextures = { sineCardTexture, cosineCardTexture, placeholder1card, placeholder2card, placeholder3card, passivedamagecard, passivespeedcard, passivedashcard };

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
