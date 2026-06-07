// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include "tree.h"

PMTree::PMTree(const std::vector<char>& src) : base(src) {
    top = new Node(0);
    top->links.clear();
    
    std::vector<char> sorted_src = src;
    std::sort(sorted_src.begin(), sorted_src.end());

    for (char ch : sorted_src) {
        std::vector<char> leftover = sorted_src;
        auto it = std::find(leftover.begin(), leftover.end(), ch);
        if (it != leftover.end()) {
            leftover.erase(it);
        }
        Node* child = generate(leftover);
        child->sym = ch;
        top->links.push_back(child);
    }
}

PMTree::~PMTree() {
    destroy(top);
}

PMTree::Node* PMTree::generate(const std::vector<char>& rest) {
    if (rest.empty()) return new Node(0);
    Node* cur = new Node(0);
    cur->links.clear();
    std::vector<char> sorted_rest = rest;
    std::sort(sorted_rest.begin(), sorted_rest.end());
    for (char ch : sorted_rest) {
        std::vector<char> leftover = sorted_rest;
        auto it = std::find(leftover.begin(), leftover.end(), ch);
        if (it != leftover.end()) {
            leftover.erase(it);
        }
        Node* child = generate(leftover);
        child->sym = ch;
        cur->links.push_back(child);
    }
    return cur;
}

void PMTree::destroy(Node* ptr) {
    if (!ptr) return;
    for (auto child : ptr->links) {
        destroy(child);
    }
    delete ptr;
}

void traverse(PMTree::Node* cur, std::vector<char>& buf,
    std::vector<std::vector<char>>& out, int level, int limit) {
    if (level == limit) {
        out.push_back(buf);
        return;
    }
    for (auto nxt : cur->links) {
        buf.push_back(nxt->sym);
        traverse(nxt, buf, out, level + 1, limit);
        buf.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(PMTree& obj) {
    std::vector<std::vector<char>> out;
    std::vector<char> track;
    int total = obj.base.size();
    for (auto first : obj.top->links) {
        track.push_back(first->sym);
        traverse(first, track, out, 1, total);
        track.pop_back();
    }
    return out;
}

std::vector<char> getPerm1(PMTree& obj, int pos) {
    auto full = getAllPerms(obj);
    if (pos <= 0 || pos > static_cast<int>(full.size())) return {};
    return full[pos - 1];
}

size_t fact(int n) {
    size_t res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

std::vector<char> getPerm2(PMTree& obj, int pos) {
    int total = obj.base.size();
    if (pos <= 0 || static_cast<size_t>(pos) > fact(total)) return {};

    std::vector<char> result;
    int remainder = pos - 1;
    
    // Собираем все символы в отсортированном порядке
    std::vector<char> available = obj.base;
    std::sort(available.begin(), available.end());

    for (int step = 0; step < total; ++step) {
        size_t block = fact(total - step - 1);
        int idx = remainder / block;
        remainder %= block;
        
        if (idx >= static_cast<int>(available.size())) return {};
        
        result.push_back(available[idx]);
        available.erase(available.begin() + idx);
    }
    return result;
}
