#pragma once

class PlayerUI
{
public:

	//コンストラクタ
	PlayerUI(Player& _player);

	//デストラクタ
	~PlayerUI(void);

	//描画
	void Draw(void);

private:

	//親
	Player& player_;
};

