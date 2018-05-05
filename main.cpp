#include <vector>
#include <exception>
#include <stdexcept>
#include <functional>
#include <iostream>
#include "Token.h"

std::vector<Token> Tokenizer(std::string InputCode);

std::vector<std::vector<Token>> Inverter(std::vector<Token> TokenizedInput);

int GetNextLevel(std::vector<Token> TokenizedInput, size_t StartIndex);

int Execute(std::vector<std::vector<Token>> InvertedInput, size_t Index = 0);

int FindSeriesOfTokens(std::vector<Token> World, std::vector<Token> Word);


int main() {
    auto Tokens = Tokenizer("(4+(3-1)+2)");
    auto InvertedTokens = Inverter(Tokens);
    std::cout << Execute(InvertedTokens) << std::endl; // Expected Output is: 6;
    return 0;
}

std::vector<Token> Tokenizer(std::string InputCode) {

    std::vector<Token> Out;
    std::string NumberLiteralString;

    for (auto &Character : InputCode) {
        switch (Character) {
            case ('('):
                if (NumberLiteralString != "")
                    Out.push_back(Token(NumberLiteralString, TokenType::NumberLiteral));
                NumberLiteralString = "";

                Out.push_back(Token("(", TokenType::CallExpression));
                break;

            case (')'):
                if (NumberLiteralString != "")
                    Out.push_back(Token(NumberLiteralString, TokenType::NumberLiteral));
                NumberLiteralString = "";

                Out.push_back(Token(")", TokenType::CallExpression));
                break;

            case ('+'):
                if (NumberLiteralString != "")
                    Out.push_back(Token(NumberLiteralString, TokenType::NumberLiteral));
                NumberLiteralString = "";

                Out.push_back(Token("+", TokenType::Operator));
                break;

            case ('-'):
                if (NumberLiteralString != "")
                    Out.push_back(Token(NumberLiteralString, TokenType::NumberLiteral));
                NumberLiteralString = "";

                Out.push_back(Token("-", TokenType::Operator));
                break;

            default:
                NumberLiteralString += Character;
                break;
        }
    }

    if (NumberLiteralString != "")
        Out.push_back(Token(NumberLiteralString, TokenType::NumberLiteral));

    return Out;

}

int Execute(std::vector<std::vector<Token>> InvertedInput, size_t Index) {
    std::vector<Token> Word = InvertedInput.at(Index);

    for (size_t I = 0; I < InvertedInput.at(Index).size(); I++) {
        auto& CurrentToken = InvertedInput.at(Index).at(I);

        if (CurrentToken.Type == TokenType::Operator) {
            int Result;
            if (CurrentToken.Value == "+")
                Result = std::stoi(InvertedInput.at(Index).at(I - 1).Value) + std::stoi(InvertedInput.at(Index).at(I + 1).Value);
            else if (CurrentToken.Value == "-")
                Result = std::stoi(InvertedInput.at(Index).at(I - 1).Value) - std::stoi(InvertedInput.at(Index).at(I + 1).Value);

            InvertedInput.at(Index).erase(InvertedInput.at(Index).begin() + I - 1);
            InvertedInput.at(Index).erase(InvertedInput.at(Index).begin() + I - 1);
            InvertedInput.at(Index).erase(InvertedInput.at(Index).begin() + I - 1);

            InvertedInput.at(Index).insert(InvertedInput.at(Index).begin() + I - 1, Token(std::to_string(Result), TokenType::NumberLiteral));
        }
    }

    if(Index + 1 >= InvertedInput.size())
        return std::stoi(InvertedInput.at(Index).at(1).Value);
    else{
        for(size_t I = Index + 1; I < InvertedInput.size(); I++){
            std::vector<Token>& World = InvertedInput.at(I);
            int LocationOfEvaluatedCallExpression = FindSeriesOfTokens(World, Word);
            InvertedInput.at(I).erase(InvertedInput.at(I).begin() + LocationOfEvaluatedCallExpression,
                                      InvertedInput.at(I).begin() + LocationOfEvaluatedCallExpression + Word.size());

            InvertedInput.at(I).insert(InvertedInput.at(I).begin() + LocationOfEvaluatedCallExpression,
                                       InvertedInput.at(Index).at(1));
        }

        return Execute(InvertedInput, Index + 1);
    }

}

std::vector<std::vector<Token>> Inverter(std::vector<Token> TokenizedInput) {
    std::vector<std::vector<Token>> Out;
    int Index = 0;
    while (Index = GetNextLevel(TokenizedInput, static_cast<size_t>(Index)), Index != -1) {
        std::vector<Token> SubOut;
        size_t SmallIndex = static_cast<size_t>(Index);
        while (SmallIndex < TokenizedInput.size()) {
            SubOut.push_back(TokenizedInput.at(SmallIndex));
            SmallIndex++;
        }
        Out.push_back(SubOut);
        Index++;
    }

    std::vector<std::vector<Token>> InvertedOut;
    for (int Index = Out.size() - 1; Index >= 0; Index--) {
        InvertedOut.push_back(Out.at(Index));
    }

    return InvertedOut;
}

int GetNextLevel(std::vector<Token> TokenizedInput, size_t StartIndex) {
    for (; StartIndex < TokenizedInput.size(); StartIndex++) {
        if (TokenizedInput.at(StartIndex).Value == "(") {
            return static_cast<int>(StartIndex);
        }
    }
    return -1;
}

int FindSeriesOfTokens(std::vector<Token> World, std::vector<Token> Word) {
    size_t Index = 0;
    for(auto& CurrentToken : Word){
        bool Correct = true;

        for(size_t I = 0; I < Word.size(); I++){
            if(I + Index >= World.size())
                return -1;

            if(World.at(Index + I).Value != Word.at(I).Value){
                Correct = false;
                break;
            }
        }

        if(Correct)
            return static_cast<int>(Index);

        Index++;
    }
}
