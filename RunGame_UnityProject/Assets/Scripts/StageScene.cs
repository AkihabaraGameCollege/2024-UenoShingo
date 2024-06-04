using System.Collections;
using TMPro;
using UnityEngine;
using UnityEngine.SceneManagement;

// ステージ画面の進行を制御します。
public class StageScene : MonoBehaviour
{
    // ステージ開始演出のスプラッシュ表示用のUIを指定します。
    [SerializeField]
    private TextMeshProUGUI stageIntroUI = null;
    // ステージ番号表示用のUIを指定します。
    [SerializeField]
    private GameObject stageNoUI = null;

    // ゲームオーバー表示用のUIを指定します。
    [SerializeField]
    private GameObject gameOverUI = null;
    // ステージクリアー表示用のUIを指定します。
    [SerializeField]
    private GameObject stageClearUI = null;

    // 自分自身のインスタンスを取得します。
    public static StageScene Instance { get; private set; } = null;

    // Start()よりも先に実行されます。
    private void Awake()
    {
        // 自分自身をインスタンスとして保存
        Instance = this;
    }

    // Start is called before the first frame update
    void Start()
    {
        // UIを非表示
        gameOverUI.SetActive(false);
        stageClearUI.SetActive(false);

        StartCoroutine(OnStart());
    }

    // ステージ開始演出を開始します。
    IEnumerator OnStart()
    {
        // StageNoの子オブジェクトからコンポーネントを検索する
        var stageNoText = stageNoUI.GetComponentInChildren<TextMeshProUGUI>();
        // スプラッシュテキストを非表示
        stageIntroUI.enabled = false;
        stageNoUI.SetActive(false);
        stageNoText.enabled = false;
        yield return new WaitForSeconds(0.5f);
        // ステージ番号を表示
        stageNoUI.SetActive(true);
        yield return new WaitForSeconds(0.2f);
        stageNoText.text = "STAGE 1";
        stageNoText.enabled = true;
        yield return new WaitForSeconds(1);
        stageNoUI.SetActive(false);
        // カウントダウン
        stageIntroUI.text = "3";
        stageIntroUI.enabled = true;
        yield return new WaitForSeconds(1);
        stageIntroUI.text = "2";
        yield return new WaitForSeconds(1);
        stageIntroUI.text = "1";
        yield return new WaitForSeconds(1);
        stageIntroUI.text = "GO!";
        yield return new WaitForSeconds(1);
        stageIntroUI.enabled = false;
    }

    // このステージをゲームオーバーとします。
    public void GameOver()
    {
        StartCoroutine(OnGameOver());
    }

    // ゲームオーバー演出を実行します。
    IEnumerator OnGameOver()
    {
        // ゲームオーバーUIを表示
        gameOverUI.SetActive(true);
        // 2秒間待機
        yield return new WaitForSeconds(2);
        // 決定ボタンの入力を待機
        while (true)
        {
            // 決定ボタンが押された場合、待機ループを抜ける
            if (Input.GetButtonDown("Submit"))
            {
                break;
            }
            yield return null;
        }
        // Stage 0シーンをリロードする
        SceneManager.LoadScene("Stage 0");
    }

    // このステージをステージクリアーとします。
    public void StageClear()
    {
        StartCoroutine(OnStageClear());
    }

    // ステージクリアー演出を実行します。
    IEnumerator OnStageClear()
    {
        // ステージクリアーUIを表示
        stageClearUI.SetActive(true);
        // 2秒間待機
        yield return new WaitForSeconds(2);
        // 決定ボタンの入力を待機
        while (true)
        {
            // 決定ボタンが押された場合、待機ループを抜ける
            if (Input.GetButtonDown("Submit"))
            {
                break;
            }
            yield return null;
        }
        // "GameClear"シーンをロードする
        SceneManager.LoadScene("GameClear");
    }
}
