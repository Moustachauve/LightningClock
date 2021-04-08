#pragma once

#include <NeoPixelBrightnessBusGfx.h>
#include <math.h>
#include "Renderer.h"

namespace Renderer {
    
    enum class PongGameState {
        init,
        active,
        score
    };

    template<typename T_PIXEL_METHOD> class Pong : public Renderer<T_PIXEL_METHOD>
    {
    private:
        const uint8_t PLAYER_HEIGHT = 3;
        const uint8_t PLAYER_OFFSET = 0;
        const float START_SPEED = 0.08;

        PongGameState state = PongGameState::init;

        unsigned long prevMillis = 0;

        float ballX = MATRIX_WIDTH / 2;
        float ballY = MATRIX_HEIGHT / 2;
        float ballSpeedX = START_SPEED;
        float ballSpeedY = START_SPEED;

        uint8_t scoreP1 = 0;
        uint8_t scoreP2 = 0;

        int8_t player1Y = (MATRIX_HEIGHT - PLAYER_HEIGHT) / 2;
        int8_t player2Y = player1Y;

        void DrawObjects()
        {
            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor(RgbwColor(0, 0, 0, 255));

            // Player 1
            Renderer<T_PIXEL_METHOD>::matrix->drawLine(PLAYER_OFFSET, player1Y, PLAYER_OFFSET, player1Y + PLAYER_HEIGHT, 0);

            // Player 2
            Renderer<T_PIXEL_METHOD>::matrix->drawLine(MATRIX_WIDTH - 1 - PLAYER_OFFSET, player2Y, MATRIX_WIDTH - 1 - PLAYER_OFFSET, player2Y + PLAYER_HEIGHT, 0);

            if (state != PongGameState::init) {
                // Ball
                Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor(RgbwColor(255, 0, 0, 255));
                Renderer<T_PIXEL_METHOD>::matrix->drawPixel(ballX, ballY, 0);
            }

            Renderer<T_PIXEL_METHOD>::matrix->setPassThruColor();
        }

        void MoveBall()
        {
            float nextBallX = ballX + ballSpeedX;

            if (round(nextBallX) <= 1 + PLAYER_OFFSET && ballY >= player1Y && ballY <= player1Y + PLAYER_HEIGHT) {
                ballSpeedX *= -1.05;
            } else if (round(nextBallX) >= MATRIX_WIDTH - 1 - PLAYER_OFFSET && ballY >= player2Y && ballY <= player2Y + PLAYER_HEIGHT) {
                ballSpeedX *= -1.05;
            }

            if (ballX <= 0 + PLAYER_OFFSET) {
                Score(scoreP2);
            } else if (ballX >= MATRIX_WIDTH - 1 - PLAYER_OFFSET) {
                Score(scoreP1);
            } 

            if (round(ballY) <= 0) {
                ballSpeedY *= -1;
            } else if (round(ballY) >= MATRIX_HEIGHT) {
                ballSpeedY *= -1;
            }
            
            ballX += ballSpeedX;
            ballY += ballSpeedY;
        }

        void ShowStart()
        {
            Renderer<T_PIXEL_METHOD>::matrix->setFont(&m3x6);
            int16_t  x1, y1;
            uint16_t w, h;
            const char* text = "START";
            Renderer<T_PIXEL_METHOD>::matrix->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
            Renderer<T_PIXEL_METHOD>::matrix->setCursor((MATRIX_WIDTH - w) / 2, 6);

            Renderer<T_PIXEL_METHOD>::matrix->print(text);

            unsigned long curMillis = millis(); 
            if (curMillis - prevMillis >= 3000) {
                state = PongGameState::active;
            }
        }

        void Score(uint8_t &scoreVariable)
        {
            state = PongGameState::score;
            scoreVariable++;
            prevMillis = millis();
            if (scoreVariable == scoreP1) {
                ballSpeedX = START_SPEED;
            } else {
                ballSpeedX = -START_SPEED;
            }
            ballSpeedY = START_SPEED;
        }

        void ResetBall()
        {
            ballSpeedX *= -1;
            ballX = MATRIX_WIDTH / 2;
            ballY = MATRIX_HEIGHT / 2;
        }

        void ShowScore()
        {
            Renderer<T_PIXEL_METHOD>::matrix->setFont(&m3x6);
            int16_t  x1, y1;
            uint16_t w, h;

            char buffer [10];
            sprintf(buffer, "%d - %d", scoreP1, scoreP2);
            Renderer<T_PIXEL_METHOD>::matrix->getTextBounds(buffer, 0, 0, &x1, &y1, &w, &h);
            Renderer<T_PIXEL_METHOD>::matrix->setCursor((MATRIX_WIDTH - w) / 2, 6);

            Renderer<T_PIXEL_METHOD>::matrix->print(buffer);

            unsigned long curMillis = millis(); 
            if (curMillis - prevMillis >= 3000) {
                state = PongGameState::active;
                ResetBall();
            }
        }

        void moveP1(int8_t direction)
        {
            moveP2(direction);
            player1Y += direction;
        }

        void moveP2(int8_t direction) 
        {
            player2Y += direction;
        }

    public:
        Pong(T_PIXEL_METHOD* pMatrix) : Renderer<T_PIXEL_METHOD>(pMatrix) 
        {
            prevMillis = millis();
        };
        virtual ~Pong() {};

        virtual void Draw()
        {
            Renderer<T_PIXEL_METHOD>::matrix->fillScreen(0);
            if (state == PongGameState::active) {
                MoveBall();
            } else if (state == PongGameState::init) {
                ShowStart();
            } else if (state == PongGameState::score) {
                ShowScore();
            }

            DrawObjects();
        };

        void OnWsEvent(AsyncWebSocket * wsServer, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) override
        {
            AwsFrameInfo *info = (AwsFrameInfo *)arg;
            if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
            {
                data[len] = 0;
                if (strcmp((char *)data, "controller-up") == 0)
                {
                    moveP1(-1);
                }
                if (strcmp((char *)data, "controller-down") == 0)
                {
                    moveP1(1);
                }
            }
        }
    };
}
