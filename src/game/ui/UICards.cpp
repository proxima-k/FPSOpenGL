#include "../ui/UICards.h"

void UICards::renderCardSelection(ImVec2 windowSize)
{
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

        if (isHovered && !cardIsAnimating[i]) {
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

        // adjust position based on scaling to keep the card centered
        ImVec2 scaleOffset = ImVec2((scaledCardSize.x - cardSize.x) / 2, (scaledCardSize.y - cardSize.y) / 2);
        ImVec2 adjustedPos = ImVec2(cardPos.x - scaleOffset.x, cardPos.y - scaleOffset.y);

        ImGui::SetCursorPos(adjustedPos);
        bool clicked = ImGui::ImageButton((void*)(intptr_t)selectedTextures[i], scaledCardSize);

        if (clicked)
        {
            std::cout << "Pressed button " << i << std::endl;

            if (selectedTextures[i] == sineCardTexture)
            {
                shooter->cardQueue.push(new SineCard(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera)));
            }
            else if (selectedTextures[i] == cosineCardTexture)
            {
                shooter->cardQueue.push(new CosineCard(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera)));
            }
            else if (selectedTextures[i] == placeholder1card)
            {
                shooter->cardQueue.push(new PlaceHolderCard1(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera)));
            }
            else if (selectedTextures[i] == placeholder2card)
            {
                shooter->cardQueue.push(new PlaceHolderCard2(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera)));
            }
            else if (selectedTextures[i] == placeholder3card)
            {
                shooter->cardQueue.push(new PlaceHolderCard3(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera)));
            }
            else if (selectedTextures[i] == passivedamagecard)
            {
                auto damagePassive = new DamagePassive(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera));

				shooter->cardPassivesQueue.push(damagePassive);
                damagePassive->applyPassiveEffect();
			}
            else if (selectedTextures[i] == passivespeedcard)
            {
                auto passivespeedcard = new SpeedPassive(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera));

                shooter->cardPassivesQueue.push(passivespeedcard);
                passivespeedcard->applyPassiveEffect();
            }
            else if (selectedTextures[i] == passivedashcard)
            {
                auto dashbuffcard = new DashPassive(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera));

                shooter->cardPassivesQueue.push(dashbuffcard);
                dashbuffcard->applyPassiveEffect();
            }
            else if (selectedTextures[i] == passivesizecard)
            {
                auto passivesizecard = new SizePassive(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera));

                shooter->cardPassivesQueue.push(passivesizecard);
                passivesizecard->applyPassiveEffect();
            }

            game->currentGameState = Game::GameStates::Playing;

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

        case Card::CardType::PassiveSize:
            ImGui::Image((void*)(intptr_t)passivesizecard, cardSize);
            break;

        default:
            ImGui::Image((void*)(intptr_t)basicCardTexture, cardSize);
            break;
        }
    }
}
void UICards::randomizeCards()
{
    std::vector<GLuint> cardTextures = 
    { sineCardTexture, cosineCardTexture, placeholder1card, placeholder2card, placeholder3card, passivedamagecard, passivespeedcard, passivedashcard, passivesizecard };

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
